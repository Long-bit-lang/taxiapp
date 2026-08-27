#include "TaxiPage.h"
#include "Style.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QDialog>
#include <QFormLayout>
#include <QMessageBox>
#include <QCheckBox>
#include <QScrollArea>
#include <algorithm>

TaxiPage::TaxiPage(HeThongTaxi* ht, QWidget* parent) : QWidget(parent), heThong(ht) {
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(20, 16, 20, 16);

    auto* lblTitle = new QLabel("QUAN LY TAXI", this);
    lblTitle->setStyleSheet("font-size:17px; font-weight:bold; color:#2C3E50;");
    root->addWidget(lblTitle);
    root->addSpacing(8);

    auto* toolbar = new QHBoxLayout();
    auto* btnThem = new QPushButton("+  Them taxi", this);
    btnThem->setObjectName("btnPrimary");
    auto* btnXoa = new QPushButton("Xoa", this);
    btnXoa->setObjectName("btnDanger");
    auto* btnPhanCong = new QPushButton("Phan cong tai xe", this);
    btnPhanCong->setObjectName("btnFlat");
    auto* btnHoanThanh = new QPushButton("Hoan thanh chuyen", this);
    btnHoanThanh->setObjectName("btnSuccess");
    auto* btnHuyChuyen = new QPushButton("Huy chuyen", this);
    btnHuyChuyen->setObjectName("btnDanger");
    connect(btnHoanThanh, &QPushButton::clicked, this, &TaxiPage::onHoanThanh);
    connect(btnThem, &QPushButton::clicked, this, &TaxiPage::onThem);
    connect(btnXoa, &QPushButton::clicked, this, &TaxiPage::onXoa);
    connect(btnPhanCong, &QPushButton::clicked, this, &TaxiPage::onPhanCong);
    connect(btnHuyChuyen, &QPushButton::clicked, this, &TaxiPage::onHuyChuyen);
    toolbar->addWidget(btnThem);
    toolbar->addWidget(btnXoa);
    toolbar->addWidget(btnPhanCong);
    toolbar->addWidget(btnHoanThanh);
    toolbar->addWidget(btnHuyChuyen);
    toolbar->addStretch();
    root->addLayout(toolbar);
    root->addSpacing(10);

    table = new QTableWidget(this);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"Bien so", "Suc chua", "Tai xe phan cong", "Trang thai"});
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->verticalHeader()->setVisible(false);
    root->addWidget(table);

    refresh();
}

void TaxiPage::refresh() {
    heThong->taxi().docFile();
    heThong->taiXe().docFile();
    const auto& ds = heThong->taxi().layDanhSach();
    table->setRowCount((int)ds.size());
    for (int i = 0; i < (int)ds.size(); ++i) {
        const auto& t = ds[i];
        table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(t.getBienSo())));
        table->setItem(i, 1, new QTableWidgetItem(QString::number(t.getSucChua()) + " cho"));

        QStringList tens;
        for (auto& maTX : t.getDsMaTaiXe()) {
            for (auto& tx : heThong->taiXe().layDanhSach())
                if (tx.getMaTX() == maTX) tens << QString::fromStdString(tx.getHoTen());
        }
        table->setItem(i, 2, new QTableWidgetItem(tens.join(", ")));

        auto* itTrangThai = new QTableWidgetItem(t.kiemTraSanSang() ? "San sang" : "Dang phuc vu");
        if (t.kiemTraSanSang()) itTrangThai->setForeground(QColor("#438F78"));
        table->setItem(i, 3, itTrangThai);
    }
}

void TaxiPage::onThem() {
    QDialog dlg(this);
    dlg.setWindowTitle("Them taxi moi");
    auto* form = new QFormLayout(&dlg);
    auto* txtBienSo = new QLineEdit(&dlg);
    auto* spinSucChua = new QSpinBox(&dlg);
    spinSucChua->setRange(2, 16);
    spinSucChua->setValue(4);
    form->addRow("Bien so xe:", txtBienSo);
    form->addRow("Suc chua:", spinSucChua);
    auto* btnRow = new QHBoxLayout();
    auto* btnCancel = new QPushButton("Huy", &dlg); btnCancel->setObjectName("btnFlat");
    auto* btnSave = new QPushButton("Luu", &dlg); btnSave->setObjectName("btnPrimary");
    connect(btnCancel, &QPushButton::clicked, &dlg, &QDialog::reject);
    connect(btnSave, &QPushButton::clicked, &dlg, &QDialog::accept);
    btnRow->addStretch(); btnRow->addWidget(btnCancel); btnRow->addWidget(btnSave);
    form->addRow(btnRow);

    if (dlg.exec() == QDialog::Accepted) {
        std::string bienSo = txtBienSo->text().toStdString();
        if (bienSo.empty()) { QMessageBox::warning(this, "Loi", "Vui long nhap bien so xe!"); return; }
        if (heThong->taxi().coBienSo(bienSo)) {
            QMessageBox::warning(this, "Loi", "Bien so nay da ton tai!");
            return;
        }
        heThong->taxi().them(Taxi(bienSo, spinSucChua->value()));
        refresh();
    }
}

void TaxiPage::onXoa() {
    int row = table->currentRow();
    if (row < 0) { QMessageBox::information(this, "Thong bao", "Vui long chon mot taxi de xoa."); return; }
    std::string bienSo = table->item(row, 0)->text().toStdString();
    if (heThong->chuyenXe().taxiDangChay(bienSo)) {
        QMessageBox::warning(this, "Khong the xoa",
                             QString("Taxi \"%1\" dang thuc hien mot chuyen xe, khong the xoa luc nay.")
                                 .arg(QString::fromStdString(bienSo)));
        return;
    }
    auto ret = QMessageBox::question(this, "Xac nhan xoa",
        QString("Ban co chac muon xoa taxi \"%1\"?").arg(QString::fromStdString(bienSo)));
    if (ret == QMessageBox::Yes) {
        heThong->taxi().xoa(bienSo);
        refresh();
    }
}

void TaxiPage::onPhanCong() {
    int row = table->currentRow();
    if (row < 0) { QMessageBox::information(this, "Thong bao", "Vui long chon mot taxi de phan cong tai xe."); return; }
    std::string bienSo = table->item(row, 0)->text().toStdString();
    Taxi* taxi = heThong->taxi().timTheoBienSo(bienSo);
    if (!taxi) return;

    QDialog dlg(this);
    dlg.setWindowTitle("Phan cong tai xe cho taxi " + QString::fromStdString(bienSo));
    dlg.setMinimumWidth(320);
    auto* lay = new QVBoxLayout(&dlg);
    lay->addWidget(new QLabel("Chon tai xe duoc phan cong lai xe nay:", &dlg));

    std::vector<QCheckBox*> checks;
    for (auto& tx : heThong->taiXe().layDanhSach()) {
        auto* cb = new QCheckBox(QString::fromStdString(tx.getMaTX() + " - " + tx.getHoTen()), &dlg);
        cb->setProperty("maTX", QString::fromStdString(tx.getMaTX()));
        bool assigned = std::find(taxi->getDsMaTaiXe().begin(), taxi->getDsMaTaiXe().end(), tx.getMaTX()) != taxi->getDsMaTaiXe().end();
        cb->setChecked(assigned);
        lay->addWidget(cb);
        checks.push_back(cb);
    }

    auto* btnRow = new QHBoxLayout();
    auto* btnCancel = new QPushButton("Huy", &dlg); btnCancel->setObjectName("btnFlat");
    auto* btnSave = new QPushButton("Luu", &dlg); btnSave->setObjectName("btnPrimary");
    connect(btnCancel, &QPushButton::clicked, &dlg, &QDialog::reject);
    connect(btnSave, &QPushButton::clicked, &dlg, &QDialog::accept);
    btnRow->addStretch(); btnRow->addWidget(btnCancel); btnRow->addWidget(btnSave);
    lay->addLayout(btnRow);

    if (dlg.exec() == QDialog::Accepted) {
        for (auto* cb : checks) {
            std::string maTX = cb->property("maTX").toString().toStdString();
            if (cb->isChecked()) taxi->themTaiXe(maTX);
            else taxi->xoaTaiXe(maTX);
        }
        heThong->taxi().ghiFile();
        refresh();
    }
}

void TaxiPage::onHoanThanh() {
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Thong bao", "Vui long chon mot taxi.");
        return;
    }
    std::string bienSo = table->item(row, 0)->text().toStdString();
    Taxi* taxi = heThong->taxi().timTheoBienSo(bienSo);
    if (!taxi) return;

    if (taxi->kiemTraSanSang()) {
        QMessageBox::information(this, "Thong bao",
                                 "Taxi nay dang o trang thai San sang, khong co chuyen nao can hoan thanh.");
        return;
    }

    auto ret = QMessageBox::question(this, "Xac nhan",
                                     QString("Xac nhan taxi \"%1\" da hoan thanh chuyen va san sang nhan khach moi?")
                                         .arg(QString::fromStdString(bienSo)));
    if (ret != QMessageBox::Yes) return;

    ChuyenXe* chuyen = heThong->chuyenXe().timChuyenDangDi(bienSo);
    if (chuyen) heThong->chuyenXe().hoanThanhChuyen(chuyen->getMaChuyen());

    taxi->setSanSang(true);
    heThong->taxi().ghiFile();
    refresh();
}

void TaxiPage::onHuyChuyen() {
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Thong bao", "Vui long chon mot taxi.");
        return;
    }
    std::string bienSo = table->item(row, 0)->text().toStdString();
    Taxi* taxi = heThong->taxi().timTheoBienSo(bienSo);
    if (!taxi) return;

    if (taxi->kiemTraSanSang()) {
        QMessageBox::information(this, "Thong bao",
                                 "Taxi nay dang o trang thai San sang, khong co chuyen nao de huy.");
        return;
    }

    ChuyenXe* chuyen = heThong->chuyenXe().timChuyenDangDi(bienSo);
    QString maChuyen = chuyen ? QString::fromStdString(chuyen->getMaChuyen()) : "khong xac dinh";

    auto ret = QMessageBox::warning(this, "Xac nhan huy chuyen",
                                    QString("Xac nhan HUY chuyen xe %1 cua taxi \"%2\"?\nHanh dong nay khong the hoan tac.")
                                        .arg(maChuyen, QString::fromStdString(bienSo)),
                                    QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (ret != QMessageBox::Yes) return;

    if (chuyen) heThong->chuyenXe().huyChuyen(chuyen->getMaChuyen());
    taxi->setSanSang(true);
    heThong->taxi().ghiFile();
    refresh();
}
