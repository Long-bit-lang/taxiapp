#include "Style.h"
#include "TripPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QFrame>
#include <QDate>
#include <QTimer>
#include <QScrollBar>
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QMessageBox>
#include <QHash>
#include <QPair>

TripPage::TripPage(HeThongTaxi* ht, QWidget* parent) : QWidget(parent), heThong(ht) {
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(20, 16, 20, 16);

    // ----- tiêu đề + ngày -----
    auto* head = new QHBoxLayout();
    auto* lblTitle = new QLabel("CHUYẾN XE HÔM NAY", this);
    lblTitle->setObjectName("pageTitle");
    lblNgay = new QLabel(this);
    lblNgay->setStyleSheet("color:#5a5a56; font-size: 15px");
    head->addWidget(lblTitle);
    head->addStretch();
    head->addWidget(lblNgay);
    root->addLayout(head);
    root->addSpacing(8);

    // ----- 3 thẻ thống kê (bấm trực tiếp để lọc nhanh) -----
    auto* cards = new QHBoxLayout();
    auto themThe = [&](QFrame*& outCard, QLabel*& outValue, const QString& nhan, const QString& mau) {
        outCard = new QFrame(this);
        outCard->setObjectName("statCard");
        outCard->setMinimumSize(190, 100);
        outCard->setCursor(Qt::PointingHandCursor);
        outCard->installEventFilter(this);
        auto* lay = new QVBoxLayout(outCard);
        outValue = new QLabel("0", outCard);
        outValue->setAlignment(Qt::AlignCenter);
        outValue->setStyleSheet(QString("font-size:28px; font-weight:bold; color:%1;").arg(mau));
        auto* lblNhan = new QLabel(nhan, outCard);
        lblNhan->setAlignment(Qt::AlignCenter);
        lblNhan->setStyleSheet("font-size:13px; color:#5a5a56;");
        lay->addWidget(outValue);
        lay->addWidget(lblNhan);
        cards->addWidget(outCard);
    };
    themThe(cardDangDi,    lblDangDi,     "Đang đi",    "#4285C7");
    themThe(cardHoanThanh, lblHoanThanh,  "Hoàn thành", "#438F78");
    themThe(cardDaHuy,     lblDaHuy,      "Đã hủy",     "#B8503F");
    cards->addStretch();
    root->addLayout(cards);
    root->addSpacing(10);

    // ----- thanh lọc + thao tác -----
    auto* bar = new QHBoxLayout();
    bar->addWidget(new QLabel("Hiển thị:", this));
    cbLoc = new QComboBox(this);
    cbLoc->addItem("Đang đi", (int)DANG_DI);
    cbLoc->addItem("Hoàn thành", (int)HOAN_THANH);
    cbLoc->addItem("Đã hủy", (int)DA_HUY);
    cbLoc->addItem("Tất cả", -1);
    bar->addWidget(cbLoc);

    bar->addSpacing(16);
    btnHoanThanh = new QPushButton("Hoàn thành chuyến", this);
    btnHoanThanh->setObjectName("btnSuccess");
    btnHoanThanh->setEnabled(false);
    btnHuyChuyen = new QPushButton("Hủy chuyến", this);
    btnHuyChuyen->setObjectName("btnDanger");
    btnHuyChuyen->setEnabled(false);
    bar->addWidget(btnHoanThanh);
    bar->addWidget(btnHuyChuyen);

    bar->addStretch();
    auto* btnLamMoi = new QPushButton("Làm mới", this);
    btnLamMoi->setObjectName("btnFlat");
    bar->addWidget(btnLamMoi);
    root->addLayout(bar);
    root->addSpacing(6);

    // ----- bảng -----
    table = new QTableWidget(this);
    table->setColumnCount(9);
    table->setHorizontalHeaderLabels({"Mã chuyến", "Thời điểm", "Biển số", "Tài xế",
                                      "SĐT tài xế", "Khách hàng", "Lộ trình", "Cước phí", "Trạng thái"});
    caiDatBang(table, 3, 6);

    auto* h = table->horizontalHeader();
    h->setSectionResizeMode(0, QHeaderView::Fixed);
    table->setColumnWidth(0, 110);
    h->setSectionResizeMode(1, QHeaderView::Fixed);
    table->setColumnWidth(1, 165);
    h->setSectionResizeMode(2, QHeaderView::Fixed);
    table->setColumnWidth(2, 120);
    h->setSectionResizeMode(3, QHeaderView::Stretch);
    h->setSectionResizeMode(4, QHeaderView::Fixed);
    table->setColumnWidth(4, 120);
    h->setSectionResizeMode(5, QHeaderView::Interactive);
    table->setColumnWidth(5, 220);
    h->setSectionResizeMode(6, QHeaderView::Stretch);
    h->setSectionResizeMode(7, QHeaderView::Fixed);
    table->setColumnWidth(7, 120);
    h->setSectionResizeMode(8, QHeaderView::Fixed);
    table->setColumnWidth(8, 120);

    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->verticalHeader()->setVisible(false);
    root->addWidget(table, 1);

    lblTrong = new QLabel("Không có chuyến xe nào phù hợp.", this);
    lblTrong->setAlignment(Qt::AlignCenter);
    lblTrong->setStyleSheet("color:#96948C; padding:12px;");
    root->addWidget(lblTrong);

    // Nhãn nổi mở rộng hiển thị trọn vẹn thông tin khi ô bị cắt "..."
    overlayCell = new QLabel(table->viewport());
    overlayCell->setObjectName("cellDetailOverlay");
    overlayCell->setWordWrap(true);
    overlayCell->setTextInteractionFlags(Qt::TextSelectableByMouse);
    overlayCell->setStyleSheet(
        "QLabel#cellDetailOverlay {"
        "  background: #FFFFFF;"
        "  color: #2C3E50;"
        "  font-size: 14px;"
        "  border: 2px solid #4285C7;"
        "  border-radius: 6px;"
        "  padding: 6px 12px;"
        "}"
    );
    auto* shadow = new QGraphicsDropShadowEffect(overlayCell);
    shadow->setBlurRadius(12);
    shadow->setColor(QColor(0, 0, 0, 70));
    shadow->setOffset(0, 3);
    overlayCell->setGraphicsEffect(shadow);
    overlayCell->hide();

    connect(table, &QTableWidget::cellClicked, this, &TripPage::onCellClicked);
    connect(table, &QTableWidget::itemSelectionChanged, this, &TripPage::capNhatTrangThaiNut);
    connect(table->verticalScrollBar(), &QScrollBar::valueChanged, this, &TripPage::hideOverlay);
    connect(table->horizontalScrollBar(), &QScrollBar::valueChanged, this, &TripPage::hideOverlay);
    table->viewport()->installEventFilter(this);

    connect(btnHoanThanh, &QPushButton::clicked, this, &TripPage::onHoanThanh);
    connect(btnHuyChuyen, &QPushButton::clicked, this, &TripPage::onHuyChuyen);

    connect(cbLoc, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]() {
        hideOverlay();
        capNhatVienTheLoc();
        refresh(false);
    });
    connect(btnLamMoi, &QPushButton::clicked, this, [this]() {
        hideOverlay();
        refresh(true);
    });

    // tự làm mới mỗi 10 giây nếu tab đang hiển thị và không đang mở chi tiết ô
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() {
        if (isVisible() && (!overlayCell || !overlayCell->isVisible())) {
            refresh(false);
        }
    });
    timer->start(10000);

    capNhatVienTheLoc();
    refresh(false);
}

bool TripPage::eventFilter(QObject* watched, QEvent* event) {
    if (event->type() == QEvent::MouseButtonPress) {
        if (watched == cardDangDi) {
            int idx = cbLoc->findData((int)DANG_DI);
            if (idx >= 0) cbLoc->setCurrentIndex(idx);
            return true;
        }
        if (watched == cardHoanThanh) {
            int idx = cbLoc->findData((int)HOAN_THANH);
            if (idx >= 0) cbLoc->setCurrentIndex(idx);
            return true;
        }
        if (watched == cardDaHuy) {
            int idx = cbLoc->findData((int)DA_HUY);
            if (idx >= 0) cbLoc->setCurrentIndex(idx);
            return true;
        }
        if (watched == table->viewport()) {
            auto* me = static_cast<QMouseEvent*>(event);
            QModelIndex idx = table->indexAt(me->pos());
            if (!idx.isValid()) {
                hideOverlay();
            }
        }
    }
    if (event->type() == QEvent::KeyPress) {
        auto* ke = static_cast<QKeyEvent*>(event);
        if (ke->key() == Qt::Key_Escape) {
            hideOverlay();
        }
    }
    return QWidget::eventFilter(watched, event);
}

void TripPage::capNhatVienTheLoc() {
    int loc = cbLoc ? cbLoc->currentData().toInt() : -1;
    auto styleThe = [](QFrame* card, bool active, const QString& color) {
        if (!card) return;
        if (active) {
            card->setStyleSheet(QString("QFrame#statCard { background: white; border: 2px solid %1; border-radius: 10px; }").arg(color));
        } else {
            card->setStyleSheet("QFrame#statCard { background: white; border: 1px solid #E4E2D8; border-radius: 10px; }");
        }
    };
    styleThe(cardDangDi,    loc == (int)DANG_DI,    "#4285C7");
    styleThe(cardHoanThanh, loc == (int)HOAN_THANH, "#438F78");
    styleThe(cardDaHuy,     loc == (int)DA_HUY,     "#B8503F");
}

void TripPage::capNhatTrangThaiNut() {
    int row = table->currentRow();
    bool choPhep = false;
    if (row >= 0 && table->item(row, 0)) {
        int trangThai = table->item(row, 0)->data(Qt::UserRole).toInt();
        choPhep = (trangThai == (int)DANG_DI);
    }
    if (btnHoanThanh) btnHoanThanh->setEnabled(choPhep);
    if (btnHuyChuyen) btnHuyChuyen->setEnabled(choPhep);
}

void TripPage::hideOverlay() {
    selectedRow = -1;
    selectedCol = -1;
    if (overlayCell) overlayCell->hide();
}

void TripPage::onCellClicked(int row, int col) {
    auto* it = table->item(row, col);
    if (!it || it->text().isEmpty()) {
        hideOverlay();
        return;
    }

    // Nếu bấm lại chính ô đang mở -> thu gọn lại
    if (selectedRow == row && selectedCol == col && overlayCell && overlayCell->isVisible()) {
        hideOverlay();
        return;
    }

    // Đóng ô cũ trước (ô cũ trở lại bình thường)
    hideOverlay();

    QRect cellRect = table->visualItemRect(it);
    if (cellRect.isNull() || !cellRect.isValid()) return;

    QFontMetrics fm(table->font());
    int textWidth = fm.horizontalAdvance(it->text());

    // Kiểm tra xem nội dung ô có bị tràn (hiện "...") hay không
    bool isTruncated = (textWidth > cellRect.width() - 20) || it->text().contains('\n');

    if (isTruncated) {
        selectedRow = row;
        selectedCol = col;

        overlayCell->setText(it->text());

        // Chiều rộng mở rộng vừa vặn (tối thiểu bằng ô, tối đa 580px)
        int targetW = qMax(cellRect.width(), qMin(textWidth + 28, 580));
        overlayCell->setFixedWidth(targetW);
        overlayCell->adjustSize();

        int x = cellRect.x();
        int y = cellRect.y();

        // Chống tràn mép phải khung nhìn
        if (x + overlayCell->width() > table->viewport()->width()) {
            x = qMax(0, table->viewport()->width() - overlayCell->width() - 6);
        }
        // Chống tràn mép dưới khung nhìn
        if (y + overlayCell->height() > table->viewport()->height()) {
            y = qMax(0, cellRect.bottom() - overlayCell->height());
        }

        overlayCell->move(x, y);
        overlayCell->show();
        overlayCell->raise();
    }
}

void TripPage::onHoanThanh() {
    int row = table->currentRow();
    if (row < 0 || !table->item(row, 0)) return;
    if (table->item(row, 0)->data(Qt::UserRole).toInt() != (int)DANG_DI) return;

    std::string maChuyen = table->item(row, 0)->text().toStdString();
    std::string bienSo = table->item(row, 2)->text().toStdString();

    auto ret = QMessageBox::question(this, "Xác nhận",
        QString("Xác nhận hoàn thành chuyến xe %1 (taxi \"%2\")?")
            .arg(QString::fromStdString(maChuyen), QString::fromStdString(bienSo)));
    if (ret != QMessageBox::Yes) return;

    if (heThong->chuyenXe().hoanThanhChuyen(maChuyen)) {
        Taxi* taxi = heThong->taxi().timTheoBienSo(bienSo);
        if (taxi && !heThong->chuyenXe().taxiDangChay(bienSo)) {
            taxi->setSanSang(true);
            heThong->taxi().ghiFile();
        }
        refresh(false);
        emit chuyenXeThayDoi();
    }
}

void TripPage::onHuyChuyen() {
    int row = table->currentRow();
    if (row < 0 || !table->item(row, 0)) return;
    if (table->item(row, 0)->data(Qt::UserRole).toInt() != (int)DANG_DI) return;

    std::string maChuyen = table->item(row, 0)->text().toStdString();
    std::string bienSo = table->item(row, 2)->text().toStdString();

    auto ret = QMessageBox::warning(this, "Xác nhận hủy chuyến",
        QString("Xác nhận HỦY chuyến xe %1 của taxi \"%2\"?\nHành động này không thể hoàn tác.")
            .arg(QString::fromStdString(maChuyen), QString::fromStdString(bienSo)),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (ret != QMessageBox::Yes) return;

    if (heThong->chuyenXe().huyChuyen(maChuyen)) {
        Taxi* taxi = heThong->taxi().timTheoBienSo(bienSo);
        if (taxi && !heThong->chuyenXe().taxiDangChay(bienSo)) {
            taxi->setSanSang(true);
            heThong->taxi().ghiFile();
        }
        refresh(false);
        emit chuyenXeThayDoi();
    }
}

void TripPage::refresh(bool reloadFromDisk) {
    hideOverlay();

    if (reloadFromDisk) {
        heThong->taiXe().docFile();
        heThong->taxi().docFile();
        heThong->chuyenXe().docFile();
    }

    const QDate homNay = QDate::currentDate();
    const std::string strHomNay = homNay.toString("dd/MM/yyyy").toStdString();
    lblNgay->setText("Ngày " + homNay.toString("dd/MM/yyyy"));

    const int loc = cbLoc->currentData().toInt();   // -1 = tất cả
    const auto& ds = heThong->chuyenXe().layDanhSach();

    int demDangDi = 0, demHoanThanh = 0, demDaHuy = 0;
    DanhSach<const ChuyenXe*> hienThi;

    // duyệt ngược: chuyến mới nhất lên đầu
    for (int i = ds.size() - 1; i >= 0; --i) {
        const ChuyenXe& c = ds[i];
        bool homNayBatDau = c.getThoiDiem().compare(0, 10, strHomNay) == 0;
        bool homNayKetThuc = c.getThoiDiemKetThuc().size() >= 10 &&
                             c.getThoiDiemKetThuc().compare(0, 10, strHomNay) == 0;
        // Hiển thị: chuyến đặt hôm nay, hoặc chuyến hoàn thành/hủy hôm nay, hoặc chuyến còn đang đi
        if (!homNayBatDau && !homNayKetThuc && c.getTrangThai() != DANG_DI) continue;

        switch (c.getTrangThai()) {
        case DANG_DI:    ++demDangDi;    break;
        case HOAN_THANH: ++demHoanThanh; break;
        default:         ++demDaHuy;     break;
        }
        if (loc == -1 || (int)c.getTrangThai() == loc) hienThi.push_back(&c);
    }

    lblDangDi->setText(QString::number(demDangDi));
    lblHoanThanh->setText(QString::number(demHoanThanh));
    lblDaHuy->setText(QString::number(demDaHuy));

    // Dựng bảng tra nhanh tài xế O(M) thay vì lặp tuyến tính theo từng dòng
    QHash<QString, QPair<QString, QString>> txInfo;
    for (const auto& tx : heThong->taiXe().layDanhSach()) {
        txInfo.insert(QString::fromStdString(tx.getMaTX()),
                      qMakePair(QString::fromStdString(tx.getMaTX() + " - " + tx.getHoTen()),
                                QString::fromStdString(tx.getSoDienThoai())));
    }

    table->setUpdatesEnabled(false);
    table->setRowCount((int)hienThi.size());
    lblTrong->setVisible(hienThi.empty());
    table->setVisible(!hienThi.empty());

    for (int r = 0; r < (int)hienThi.size(); ++r) {
        const ChuyenXe& c = *hienThi[r];

        QString tenTX = "(đã xóa)", sdtTX = "-";
        if (c.getMaTaiXe().empty()) {
            tenTX = "(chưa gán)";
        } else {
            auto itTx = txInfo.constFind(QString::fromStdString(c.getMaTaiXe()));
            if (itTx != txInfo.constEnd()) {
                tenTX = itTx.value().first;
                sdtTX = itTx.value().second;
            }
        }

        QString thoiDiemHienThi = QString::fromStdString(c.getThoiDiem());
        if (!c.getThoiDiemKetThuc().empty()) {
            thoiDiemHienThi += QString("  (Cập nhật: %1)")
                                   .arg(QString::fromStdString(c.getThoiDiemKetThuc()));
        }

        QString lotrinh = QString::fromStdString(c.getDiemDon() + "  →  " + c.getDiemDen())
                          + QString("  (%1 km)").arg(c.getKhoangCach(), 0, 'f', 1);

        auto setItem = [this, r](int col, const QString& text, const QColor& fg = QColor()) {
            auto* it = table->item(r, col);
            if (!it) {
                it = new QTableWidgetItem(text);
                table->setItem(r, col, it);
            } else {
                it->setText(text);
            }
            it->setToolTip(text);
            if (fg.isValid()) it->setForeground(fg);
            else it->setForeground(QColor("#3d3d3a"));
            return it;
        };

        auto* itMa = setItem(0, QString::fromStdString(c.getMaChuyen()));
        itMa->setData(Qt::UserRole, (int)c.getTrangThai());

        setItem(1, thoiDiemHienThi);
        setItem(2, QString::fromStdString(c.getBienSoTaxi()));
        setItem(3, tenTX);
        setItem(4, sdtTX);
        setItem(5, QString::fromStdString(c.getTenKhachHang() + " - " + c.getSdtKhachHang()));
        setItem(6, lotrinh);
        setItem(7, dinhDangTien(c.getCuocPhi()));

        QColor colorTT;
        switch (c.getTrangThai()) {
        case DANG_DI:    colorTT = QColor("#4285C7"); break;
        case HOAN_THANH: colorTT = QColor("#438F78"); break;
        default:         colorTT = QColor("#B8503F"); break;
        }
        setItem(8, QString::fromStdString(trangThaiChuyenToStr(c.getTrangThai())), colorTT);
    }

    table->setUpdatesEnabled(true);
    capNhatTrangThaiNut();
}
