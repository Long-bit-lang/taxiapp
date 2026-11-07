#include "Style.h"
#include "TaxiPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDialog>
#include <QFormLayout>
#include <QMessageBox>
#include <QSet>
#include <algorithm>

// ============================================================================
// Hộp thoại Phân công tài xế (Tối ưu hóa: Dùng bảng thay vì 1.300 CheckBox riêng)
// ============================================================================
class AssignDriverDialog : public QDialog {
public:
    AssignDriverDialog(HeThongTaxi* ht, Taxi* taxi, QWidget* parent = nullptr)
        : QDialog(parent), heThong(ht), taxi(taxi) {
        setWindowTitle("Phân công tài xế — Taxi " + QString::fromStdString(taxi->getBienSo()));
        resize(750, 560);

        auto* root = new QVBoxLayout(this);
        root->setContentsMargins(18, 16, 18, 16);
        root->setSpacing(10);

        // Khung thông tin xe
        QString blReq = "B2, C, D, E";
        if (taxi->getSucChua() > 29) blReq = "E";
        else if (taxi->getSucChua() > 8) blReq = "D, E";

        auto* lblInfo = new QLabel(
            QString("<b>Biển số:</b> %1 &nbsp;&nbsp;|&nbsp;&nbsp; <b>Sức chứa:</b> %2 chỗ &nbsp;&nbsp;|&nbsp;&nbsp; <b>Bằng lái hợp lệ:</b> %3")
                .arg(QString::fromStdString(taxi->getBienSo()))
                .arg(taxi->getSucChua())
                .arg(blReq), this);
        lblInfo->setStyleSheet("font-size: 14px; color: #2C3E50; background: #DFEBF7; padding: 10px 14px; border-radius: 6px;");
        root->addWidget(lblInfo);

        // Thanh công cụ lọc & tìm kiếm
        auto* filterRow = new QHBoxLayout();
        txtSearch = new QLineEdit(this);
        txtSearch->setPlaceholderText("Tìm kiếm: Mã TX, họ tên, bằng lái...");
        txtSearch->setClearButtonEnabled(true);
        filterRow->addWidget(txtSearch, 1);

        cbFilter = new QComboBox(this);
        cbFilter->addItem("Tất cả tài xế", 0);
        cbFilter->addItem("Chỉ hiện đã chọn", 1);
        cbFilter->addItem("Chỉ hiện đủ điều kiện", 2);
        filterRow->addWidget(cbFilter);

        lblCount = new QLabel(this);
        lblCount->setStyleSheet("font-weight: bold; color: #4285C7; font-size: 14px; padding-left: 8px;");
        filterRow->addWidget(lblCount);

        root->addLayout(filterRow);

        // Bảng tài xế
        table = new QTableWidget(this);
        table->setColumnCount(5);
        table->setHorizontalHeaderLabels({"Chọn", "Mã TX", "Họ và tên", "Bằng lái", "Điều kiện"});
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->verticalHeader()->setVisible(false);
        table->verticalHeader()->setDefaultSectionSize(40);
        table->setAlternatingRowColors(true);
        table->setShowGrid(false);

        auto* h = table->horizontalHeader();
        h->setSectionResizeMode(0, QHeaderView::Fixed);
        table->setColumnWidth(0, 60);
        h->setSectionResizeMode(1, QHeaderView::Fixed);
        table->setColumnWidth(1, 90);
        h->setSectionResizeMode(2, QHeaderView::Stretch);
        h->setSectionResizeMode(3, QHeaderView::Fixed);
        table->setColumnWidth(3, 85);
        h->setSectionResizeMode(4, QHeaderView::Fixed);
        table->setColumnWidth(4, 180);

        root->addWidget(table, 1);

        // Nạp các tài xế hiện đang được phân công
        for (const auto& ma : taxi->getDsMaTaiXe()) {
            selectedMaTX.insert(QString::fromStdString(ma));
        }

        buildTableData();
        updateCountLabel();

        // Thanh nút bấm ở dưới cùng (luôn hiển thị rõ ràng trên màn hình)
        auto* btnRow = new QHBoxLayout();
        auto* btnClearAll = new QPushButton("Bỏ chọn tất cả", this);
        btnClearAll->setObjectName("btnFlat");
        connect(btnClearAll, &QPushButton::clicked, this, [this]() {
            selectedMaTX.clear();
            for (int r = 0; r < table->rowCount(); ++r) {
                auto* it = table->item(r, 0);
                if (it && (it->flags() & Qt::ItemIsEnabled)) it->setCheckState(Qt::Unchecked);
            }
            updateCountLabel();
            applyFilter();
        });
        btnRow->addWidget(btnClearAll);
        btnRow->addStretch();

        auto* btnCancel = new QPushButton("Hủy", this);
        btnCancel->setObjectName("btnFlat");
        auto* btnSave = new QPushButton("Lưu phân công", this);
        btnSave->setObjectName("btnPrimary");

        connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);
        connect(btnSave, &QPushButton::clicked, this, &QDialog::accept);
        btnRow->addWidget(btnCancel);
        btnRow->addWidget(btnSave);

        root->addLayout(btnRow);

        connect(txtSearch, &QLineEdit::textChanged, this, &AssignDriverDialog::applyFilter);
        connect(cbFilter, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &AssignDriverDialog::applyFilter);
        connect(table, &QTableWidget::itemClicked, this, &AssignDriverDialog::onItemClicked);
    }

    const QSet<QString>& getSelectedMaTX() const { return selectedMaTX; }

private:
    HeThongTaxi* heThong;
    Taxi* taxi;
    QLineEdit* txtSearch;
    QComboBox* cbFilter;
    QLabel* lblCount;
    QTableWidget* table;
    QSet<QString> selectedMaTX;

    void updateCountLabel() {
        lblCount->setText(QString("Đã chọn: %1 tài xế").arg(selectedMaTX.size()));
    }

    void buildTableData() {
        const auto& ds = heThong->taiXe().layDanhSach();
        table->setRowCount((int)ds.size());
        table->setUpdatesEnabled(false);

        for (int i = 0; i < (int)ds.size(); ++i) {
            const auto& tx = ds[i];
            QString ma = QString::fromStdString(tx.getMaTX());
            QString ten = QString::fromStdString(tx.getHoTen());
            QString bl = QString::fromStdString(tx.getBangLai());
            bool phuHop = hangBangLaiPhuHopSucChua(tx.getBangLai(), taxi->getSucChua());
            bool isChecked = selectedMaTX.contains(ma);

            auto* itCheck = new QTableWidgetItem();
            itCheck->setData(Qt::UserRole, ma);
            itCheck->setData(Qt::UserRole + 1, phuHop);

            if (phuHop) {
                itCheck->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
                itCheck->setCheckState(isChecked ? Qt::Checked : Qt::Unchecked);
            } else {
                itCheck->setFlags(Qt::ItemIsSelectable);
                itCheck->setCheckState(Qt::Unchecked);
                selectedMaTX.remove(ma); // Tự động loại bỏ nếu bằng lái không phù hợp
            }
            table->setItem(i, 0, itCheck);

            auto* itMa = new QTableWidgetItem(ma);
            auto* itTen = new QTableWidgetItem(ten);
            auto* itBl = new QTableWidgetItem(bl);
            auto* itDk = new QTableWidgetItem(phuHop ? "Đủ điều kiện" : "Không đủ điều kiện");

            if (!phuHop) {
                QColor red("#B8503F");
                itMa->setForeground(red);
                itTen->setForeground(red);
                itBl->setForeground(red);
                itDk->setForeground(red);
            } else {
                itDk->setForeground(QColor("#438F78"));
            }

            table->setItem(i, 1, itMa);
            table->setItem(i, 2, itTen);
            table->setItem(i, 3, itBl);
            table->setItem(i, 4, itDk);
        }
        table->setUpdatesEnabled(true);
    }

    void onItemClicked(QTableWidgetItem* item) {
        if (!item) return;
        int row = item->row();
        auto* itCheck = table->item(row, 0);
        if (!itCheck || !(itCheck->flags() & Qt::ItemIsEnabled)) return;

        // Nhấn vào hàng bất kỳ sẽ bật/tắt checkbox
        if (item->column() != 0) {
            bool checked = (itCheck->checkState() == Qt::Checked);
            itCheck->setCheckState(checked ? Qt::Unchecked : Qt::Checked);
        }

        QString ma = itCheck->data(Qt::UserRole).toString();
        if (itCheck->checkState() == Qt::Checked) {
            selectedMaTX.insert(ma);
        } else {
            selectedMaTX.remove(ma);
        }
        updateCountLabel();
    }

    void applyFilter() {
        QString kw = txtSearch->text().trimmed().toLower();
        int filterMode = cbFilter->currentData().toInt();

        table->setUpdatesEnabled(false);
        for (int r = 0; r < table->rowCount(); ++r) {
            auto* itCheck = table->item(r, 0);
            QString ma = itCheck->data(Qt::UserRole).toString();
            bool phuHop = itCheck->data(Qt::UserRole + 1).toBool();
            bool isChecked = selectedMaTX.contains(ma);

            bool matchMode = true;
            if (filterMode == 1 && !isChecked) matchMode = false;
            else if (filterMode == 2 && !phuHop) matchMode = false;

            bool matchKw = true;
            if (!kw.isEmpty()) {
                QString ten = table->item(r, 2)->text().toLower();
                QString bl = table->item(r, 3)->text().toLower();
                if (!ma.toLower().contains(kw) && !ten.contains(kw) && !bl.contains(kw)) {
                    matchKw = false;
                }
            }

            table->setRowHidden(r, !(matchMode && matchKw));
        }
        table->setUpdatesEnabled(true);
    }
};

// ============================================================================
// Trang Quản lý Taxi
// ============================================================================
TaxiPage::TaxiPage(HeThongTaxi* ht, QWidget* parent) : QWidget(parent), heThong(ht) {
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(20, 16, 20, 16);

    auto* lblTitle = new QLabel("QUẢN LÝ TAXI", this);
    lblTitle->setObjectName("pageTitle");
    root->addWidget(lblTitle);
    root->addSpacing(8);

    // Hàng nút chức năng chính
    auto* toolbar = new QHBoxLayout();
    auto* btnThem = new QPushButton("+  Thêm taxi", this);
    btnThem->setObjectName("btnPrimary");
    auto* btnXoa = new QPushButton("Xóa taxi", this);
    btnXoa->setObjectName("btnDanger");
    auto* btnPhanCong = new QPushButton("Phân công tài xế", this);
    btnPhanCong->setObjectName("btnFlat");
    auto* btnHoanThanh = new QPushButton("Hoàn thành chuyến", this);
    btnHoanThanh->setObjectName("btnSuccess");
    auto* btnHuyChuyen = new QPushButton("Hủy chuyến", this);
    btnHuyChuyen->setObjectName("btnDanger");

    connect(btnThem, &QPushButton::clicked, this, &TaxiPage::onThem);
    connect(btnXoa, &QPushButton::clicked, this, &TaxiPage::onXoa);
    connect(btnPhanCong, &QPushButton::clicked, this, &TaxiPage::onPhanCong);
    connect(btnHoanThanh, &QPushButton::clicked, this, &TaxiPage::onHoanThanh);
    connect(btnHuyChuyen, &QPushButton::clicked, this, &TaxiPage::onHuyChuyen);

    toolbar->addWidget(btnThem);
    toolbar->addWidget(btnXoa);
    toolbar->addWidget(btnPhanCong);
    toolbar->addWidget(btnHoanThanh);
    toolbar->addWidget(btnHuyChuyen);
    toolbar->addStretch();
    root->addLayout(toolbar);
    root->addSpacing(8);

    // Hàng lọc & tìm kiếm (Tìm nhanh theo biển số, tên tài xế, trạng thái)
    auto* searchRow = new QHBoxLayout();
    cbLocTrangThai = new QComboBox(this);
    cbLocTrangThai->addItem("Tất cả trạng thái", -1);
    cbLocTrangThai->addItem("Sẵn sàng", 1);
    cbLocTrangThai->addItem("Đang phục vụ", 0);
    searchRow->addWidget(cbLocTrangThai);

    txtSearch = new QLineEdit(this);
    txtSearch->setPlaceholderText("Tìm kiếm: Biển số xe, tên tài xế...");
    txtSearch->setClearButtonEnabled(true);
    txtSearch->setFixedWidth(360);
    searchRow->addWidget(txtSearch);
    searchRow->addStretch();

    connect(txtSearch, &QLineEdit::textChanged, this, &TaxiPage::onTimKiem);
    connect(cbLocTrangThai, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &TaxiPage::onTimKiem);

    root->addLayout(searchRow);
    root->addSpacing(10);

    // Bảng danh sách taxi
    table = new QTableWidget(this);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"Biển số", "Sức chứa", "Tài xế phân công", "Trạng thái"});
    caiDatBang(table, 2);

    auto* h = table->horizontalHeader();
    h->setSectionResizeMode(0, QHeaderView::Fixed);
    table->setColumnWidth(0, 140);
    h->setSectionResizeMode(1, QHeaderView::Fixed);
    table->setColumnWidth(1, 110);
    h->setSectionResizeMode(2, QHeaderView::Stretch);
    h->setSectionResizeMode(3, QHeaderView::Fixed);
    table->setColumnWidth(3, 150);

    root->addWidget(table, 1);

    updateDriverLookup();
    fillTable();
}

void TaxiPage::updateDriverLookup() {
    tenTheoMa.clear();
    for (const auto& tx : heThong->taiXe().layDanhSach()) {
        tenTheoMa.insert(QString::fromStdString(tx.getMaTX()),
                         QString::fromStdString(tx.getHoTen()));
    }
}

void TaxiPage::refresh() {
    // Không đọc lại đĩa thừa thãi, cập nhật từ bộ nhớ chia sẻ
    updateDriverLookup();
    fillTable();
}

void TaxiPage::onTimKiem() {
    fillTable();
}

void TaxiPage::fillTable() {
    QString kw = txtSearch ? txtSearch->text().trimmed().toLower() : "";
    int statusFilter = cbLocTrangThai ? cbLocTrangThai->currentData().toInt() : -1;

    // Lọc danh sách taxi phù hợp
    DanhSach<const Taxi*> hienThi;
    for (const auto& t : heThong->taxi().layDanhSach()) {
        if (statusFilter != -1) {
            bool ss = (statusFilter == 1);
            if (t.kiemTraSanSang() != ss) continue;
        }

        if (!kw.isEmpty()) {
            QString bs = QString::fromStdString(t.getBienSo()).toLower();
            bool match = bs.contains(kw);
            if (!match) {
                for (const auto& maTX : t.getDsMaTaiXe()) {
                    auto it = tenTheoMa.constFind(QString::fromStdString(maTX));
                    if (it != tenTheoMa.constEnd() && it.value().toLower().contains(kw)) {
                        match = true;
                        break;
                    }
                }
            }
            if (!match) continue;
        }

        hienThi.themCuoi(&t);
    }

    table->setUpdatesEnabled(false);
    table->setRowCount((int)hienThi.size());

    for (int i = 0; i < (int)hienThi.size(); ++i) {
        const auto& t = *hienThi[i];

        // Tái sử dụng QTableWidgetItem nếu đã có sẵn thay vì new mới liên tục
        auto setItemText = [this, i](int col, const QString& text, const QColor& fg = QColor()) {
            auto* it = table->item(i, col);
            if (!it) {
                it = new QTableWidgetItem(text);
                table->setItem(i, col, it);
            } else {
                it->setText(text);
            }
            if (fg.isValid()) it->setForeground(fg);
            else it->setForeground(QColor("#3d3d3a"));
        };

        setItemText(0, QString::fromStdString(t.getBienSo()));
        setItemText(1, QString::number(t.getSucChua()) + " chỗ");

        QStringList tens;
        for (const auto& maTX : t.getDsMaTaiXe()) {
            auto it = tenTheoMa.constFind(QString::fromStdString(maTX));
            if (it != tenTheoMa.constEnd()) tens << it.value();
        }
        setItemText(2, tens.join(", "));

        bool ss = t.kiemTraSanSang();
        setItemText(3, ss ? "Sẵn sàng" : "Đang phục vụ", ss ? QColor("#438F78") : QColor("#4285C7"));
    }

    table->setUpdatesEnabled(true);
}

void TaxiPage::onThem() {
    QDialog dlg(this);
    dlg.setWindowTitle("Thêm taxi mới");
    auto* form = new QFormLayout(&dlg);
    auto* txtBienSo = new QLineEdit(&dlg);
    txtBienSo->setPlaceholderText("VD: 43A-99999");
    auto* spinSucChua = new QSpinBox(&dlg);
    spinSucChua->setRange(2, 16);
    spinSucChua->setValue(4);
    form->addRow("Biển số xe:", txtBienSo);
    form->addRow("Sức chứa:", spinSucChua);

    auto* btnRow = new QHBoxLayout();
    auto* btnCancel = new QPushButton("Hủy", &dlg); btnCancel->setObjectName("btnFlat");
    auto* btnSave = new QPushButton("Lưu", &dlg); btnSave->setObjectName("btnPrimary");
    connect(btnCancel, &QPushButton::clicked, &dlg, &QDialog::reject);
    connect(btnSave, &QPushButton::clicked, &dlg, &QDialog::accept);
    btnRow->addStretch(); btnRow->addWidget(btnCancel); btnRow->addWidget(btnSave);
    form->addRow(btnRow);

    if (dlg.exec() == QDialog::Accepted) {
        std::string bienSo = txtBienSo->text().trimmed().toStdString();
        if (bienSo.empty()) { QMessageBox::warning(this, "Lỗi", "Vui lòng nhập biển số xe!"); return; }
        if (heThong->taxi().coBienSo(bienSo)) {
            QMessageBox::warning(this, "Lỗi", "Biển số này đã tồn tại!");
            return;
        }
        heThong->taxi().them(Taxi(bienSo, spinSucChua->value()));
        fillTable();
    }
}

void TaxiPage::onXoa() {
    int row = table->currentRow();
    if (row < 0) { QMessageBox::information(this, "Thông báo", "Vui lòng chọn một taxi để xóa."); return; }
    std::string bienSo = table->item(row, 0)->text().toStdString();
    if (heThong->chuyenXe().taxiDangChay(bienSo)) {
        QMessageBox::warning(this, "Không thể xóa",
                             QString("Taxi \"%1\" đang thực hiện một chuyến xe, không thể xóa lúc này.")
                                 .arg(QString::fromStdString(bienSo)));
        return;
    }
    auto ret = QMessageBox::question(this, "Xác nhận xóa",
        QString("Bạn có chắc muốn xóa taxi \"%1\"?").arg(QString::fromStdString(bienSo)));
    if (ret == QMessageBox::Yes) {
        heThong->taxi().xoa(bienSo);
        fillTable();
    }
}

void TaxiPage::onPhanCong() {
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Thông báo", "Vui lòng chọn một taxi để phân công tài xế.");
        return;
    }
    std::string bienSo = table->item(row, 0)->text().toStdString();
    Taxi* taxi = heThong->taxi().timTheoBienSo(bienSo);
    if (!taxi) return;

    AssignDriverDialog dlg(heThong, taxi, this);
    if (dlg.exec() == QDialog::Accepted) {
        DanhSach<string> dsMoi;
        for (const auto& ma : dlg.getSelectedMaTX()) {
            dsMoi.themCuoi(ma.toStdString());
        }
        taxi->datDanhSachTaiXe(dsMoi);
        heThong->taxi().ghiFile();
        fillTable();
    }
}

void TaxiPage::onHoanThanh() {
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Thông báo", "Vui lòng chọn một taxi.");
        return;
    }
    std::string bienSo = table->item(row, 0)->text().toStdString();
    Taxi* taxi = heThong->taxi().timTheoBienSo(bienSo);
    if (!taxi) return;

    if (taxi->kiemTraSanSang()) {
        QMessageBox::information(this, "Thông báo",
                                 "Taxi này đang ở trạng thái sẵn sàng, không có chuyến nào cần hoàn thành.");
        return;
    }

    auto ret = QMessageBox::question(this, "Xác nhận",
                                     QString("Xác nhận taxi \"%1\" đã hoàn thành chuyến và sẵn sàng nhận khách mới?")
                                         .arg(QString::fromStdString(bienSo)));
    if (ret != QMessageBox::Yes) return;

    ChuyenXe* chuyen = heThong->chuyenXe().timChuyenDangDi(bienSo);
    if (chuyen) heThong->chuyenXe().hoanThanhChuyen(chuyen->getMaChuyen());

    taxi->setSanSang(true);
    heThong->taxi().ghiFile();
    fillTable();
}

void TaxiPage::onHuyChuyen() {
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Thông báo", "Vui lòng chọn một taxi.");
        return;
    }
    std::string bienSo = table->item(row, 0)->text().toStdString();
    Taxi* taxi = heThong->taxi().timTheoBienSo(bienSo);
    if (!taxi) return;

    if (taxi->kiemTraSanSang()) {
        QMessageBox::information(this, "Thông báo",
                                 "Taxi này đang ở trạng thái sẵn sàng, không có chuyến nào để hủy.");
        return;
    }

    ChuyenXe* chuyen = heThong->chuyenXe().timChuyenDangDi(bienSo);
    QString maChuyen = chuyen ? QString::fromStdString(chuyen->getMaChuyen()) : "không xác định";

    auto ret = QMessageBox::warning(this, "Xác nhận hủy chuyến",
                                    QString("Xác nhận HỦY chuyến xe %1 của taxi \"%2\"?\nHành động này không thể hoàn tác.")
                                        .arg(maChuyen, QString::fromStdString(bienSo)),
                                    QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (ret != QMessageBox::Yes) return;

    if (chuyen) heThong->chuyenXe().huyChuyen(chuyen->getMaChuyen());
    taxi->setSanSang(true);
    heThong->taxi().ghiFile();
    fillTable();
}
