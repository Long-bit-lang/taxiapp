#include "StatsPage.h"
#include "Style.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QFrame>

StatsPage::StatsPage(HeThongTaxi* ht, QWidget* parent) : QWidget(parent), heThong(ht) {
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(20, 16, 20, 16);

    auto* lblTitle = new QLabel("THỐNG KÊ / BÁO CÁO — Bảng lương theo taxi", this);
    lblTitle->setStyleSheet("font-size:17px; font-weight:bold; color:#2C3E50;");
    root->addWidget(lblTitle);
    root->addSpacing(8);

    auto* row = new QHBoxLayout();
    row->addWidget(new QLabel("Chọn taxi:", this));
    cbTaxi = new QComboBox(this);
    cbTaxi->setFixedWidth(220);
    connect(cbTaxi, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &StatsPage::onChonTaxi);
    row->addWidget(cbTaxi);
    row->addStretch();
    root->addLayout(row);
    root->addSpacing(10);

    table = new QTableWidget(this);
    table->setColumnCount(6);
    table->setHorizontalHeaderLabels({"Mã chuyến", "Điểm đón", "Điểm đến", "Khoảng cách", "Cước phí", "Trạng thái"});
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->verticalHeader()->setVisible(false);
    root->addWidget(table);

    auto* totalCard = new QFrame(this);
    totalCard->setObjectName("card");
    auto* totalLay = new QHBoxLayout(totalCard);
    totalLay->addWidget(new QLabel("<b>TỔNG CỘNG</b>", totalCard));
    totalLay->addStretch();
    lblTong = new QLabel("0 d", totalCard);
    lblTong->setStyleSheet("font-size:15px; font-weight:bold; color:#4285C7;");
    totalLay->addWidget(lblTong);
    root->addWidget(totalCard);

    refresh();
}

void StatsPage::refresh() {
    heThong->taxi().docFile();
    heThong->chuyenXe().docFile();
    cbTaxi->clear();
    for (auto& t : heThong->taxi().layDanhSach())
        cbTaxi->addItem(QString::fromStdString(t.getBienSo()));
    if (cbTaxi->count() > 0) onChonTaxi(0);
}

void StatsPage::onChonTaxi(int index) {
    if (index < 0 || index >= cbTaxi->count()) { table->setRowCount(0); lblTong->setText("0 d"); return; }
    std::string bienSo = cbTaxi->itemText(index).toStdString();
    auto ds = heThong->chuyenXe().theoTaxi(bienSo);
    table->setRowCount((int)ds.size());
    double tong = 0;
    for (int i = 0; i < (int)ds.size(); ++i) {
        auto& c = ds[i];
        table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(c.getMaChuyen())));
        table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(c.getDiemDon())));
        table->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(c.getDiemDen())));
        table->setItem(i, 3, new QTableWidgetItem(QString::number(c.getKhoangCach(), 'f', 1) + " km"));
        table->setItem(i, 4, new QTableWidgetItem(QString::number(c.getCuocPhi(), 'f', 0) + " đ"));

        auto* itTrangThai = new QTableWidgetItem(QString::fromStdString(trangThaiChuyenToStr(c.getTrangThai())));
        if (c.getTrangThai() == DA_HUY) itTrangThai->setForeground(QColor("#B8503F"));
        else if (c.getTrangThai() == HOAN_THANH) itTrangThai->setForeground(QColor("#438F78"));
        table->setItem(i, 5, itTrangThai);

        if (c.getTrangThai() != DA_HUY) tong += c.getCuocPhi();
    }
    lblTong->setText(QString::number(tong, 'f', 0) + " đ");
}
