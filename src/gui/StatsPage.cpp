#include "Style.h"
#include "StatsPage.h"
#include <QSpinBox>
#include "../../include/TinhLuong.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QFrame>
#include <QDate>
#include <QSignalBlocker>

StatsPage::StatsPage(HeThongTaxi* ht, QWidget* parent) : QWidget(parent), heThong(ht) {
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(20, 16, 20, 16);

    auto* lblTitle = new QLabel("THỐNG KÊ / BÁO CÁO — Bảng lương tài xế", this);
    lblTitle->setObjectName("pageTitle");
    root->addWidget(lblTitle);
    root->addSpacing(8);

    auto* row = new QHBoxLayout();
    row->addWidget(new QLabel("Năm:", this));
    spinNam = new QSpinBox(this);
    row->addWidget(spinNam);
    row->addSpacing(12);
    row->addWidget(new QLabel("Tháng:", this));
    cbThang = new QComboBox(this);
    row->addWidget(cbThang);
    row->addStretch();
    root->addLayout(row);

    spinNam->setRange(2020, QDate::currentDate().year());
    spinNam->setValue(QDate::currentDate().year());
    napDanhSachThang();

    connect(cbThang, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &StatsPage::onLoc);
    connect(spinNam, QOverload<int>::of(&QSpinBox::valueChanged), this, [this]() {
        napDanhSachThang();   // đổi năm thì nạp lại danh sách tháng
        onLoc();
    });

    table = new QTableWidget(this);
    table->setColumnCount(9);
    table->setHorizontalHeaderLabels({"Mã TX", "Họ và tên", "Số chuyến", "Tổng km",
                                      "Doanh thu", "Chiết khấu", "Phí nền tảng", "Thưởng", "Thực nhận"});
    caiDatBang(table, 1);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->verticalHeader()->setVisible(false);
    root->addWidget(table, 1);

    auto* totalCard = new QFrame(this);
    totalCard->setObjectName("card");
    auto* totalLay = new QHBoxLayout(totalCard);
    totalLay->addWidget(new QLabel("<b>TỔNG CỘNG</b>", totalCard));
    totalLay->addStretch();
    lblTong = new QLabel("0 đ", totalCard);
    lblTong->setStyleSheet("font-size:20px; font-weight:bold; color:#4285C7;");
    totalLay->addWidget(lblTong);
    root->addWidget(totalCard);

    refresh();
}

void StatsPage::capNhatGioiHan() {
    QSignalBlocker chan(spinNam);
    spinNam->setRange(2020, QDate::currentDate().year());   // tự kẹp giá trị nếu vượt giới hạn
}

void StatsPage::napDanhSachThang() {
    QDate homNay = QDate::currentDate();
    int thangToiDa = (spinNam->value() >= homNay.year()) ? homNay.month() : 12;
    int dangChon = cbThang->currentIndex() >= 0 ? cbThang->currentData().toInt() : 0;

    QSignalBlocker chan(cbThang);
    cbThang->clear();
    cbThang->addItem("Cả năm", 0);
    for (int m = 1; m <= thangToiDa; ++m)
        cbThang->addItem(QString("Tháng %1").arg(m), m);

    int idx = cbThang->findData(dangChon);
    cbThang->setCurrentIndex(idx >= 0 ? idx : 0);   // tháng cũ không còn hợp lệ thì về "Cả năm"
}

void StatsPage::refresh() {
    heThong->taiXe().docFile();
    heThong->taxi().docFile();
    heThong->chuyenXe().docFile();
    capNhatGioiHan();
    napDanhSachThang();
    onLoc();
}

void StatsPage::onLoc() {
    auto ds = tinhBangLuong(heThong->chuyenXe(), heThong->taxi(),
                            cbThang->currentData().toInt(), spinNam->value());
    table->setRowCount((int)ds.size());
    table->setUpdatesEnabled(false);
    double tongThucNhan = 0;
    for (int i = 0; i < (int)ds.size(); ++i) {
        const auto& l = ds[i];
        QString hoTen = "(đã xóa)";
        for (auto& tx : heThong->taiXe().layDanhSach())
            if (tx.getMaTX() == l.maTX) { hoTen = QString::fromStdString(tx.getHoTen()); break; }

        auto tien = [](double v) { return new QTableWidgetItem(dinhDangTien(v)); };
        table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(l.maTX)));
        table->setItem(i, 1, new QTableWidgetItem(hoTen));
        table->setItem(i, 2, new QTableWidgetItem(QString::number(l.soChuyen)));
        table->setItem(i, 3, new QTableWidgetItem(QString::number(l.tongKm, 'f', 1) + " km"));
        table->setItem(i, 4, tien(l.doanhThu));
        table->setItem(i, 5, tien(-l.chietKhau));
        table->setItem(i, 6, tien(-l.phiNenTang));
        table->setItem(i, 7, tien(l.thuong));
        auto* it = tien(l.thucNhan);
        it->setForeground(QColor("#438F78"));
        table->setItem(i, 8, it);
        tongThucNhan += l.thucNhan;
    }
    lblTong->setText(dinhDangTien(tongThucNhan));
    table->setUpdatesEnabled(true);
    vuaNoiDung(table);
}
