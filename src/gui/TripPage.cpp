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
#include <QSignalBlocker>

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

    // ----- 3 thẻ thống kê -----
    auto* cards = new QHBoxLayout();
    auto themThe = [&](QLabel*& outValue, const QString& nhan, const QString& mau) {
        auto* card = new QFrame(this);
        card->setObjectName("statCard");
        card->setMinimumSize(190, 100);
        auto* lay = new QVBoxLayout(card);
        outValue = new QLabel("0", card);
        outValue->setAlignment(Qt::AlignCenter);
        outValue->setStyleSheet(QString("font-size:28px; font-weight:bold; color:%1;").arg(mau));
        auto* lblNhan = new QLabel(nhan, card);
        lblNhan->setAlignment(Qt::AlignCenter);
        lblNhan->setStyleSheet("font-size:13px; color:#5a5a56;");
        lay->addWidget(outValue);
        lay->addWidget(lblNhan);
        cards->addWidget(card);
    };
    themThe(lblDangDi,     "Đang đi",    "#4285C7");
    themThe(lblHoanThanh,  "Hoàn thành", "#438F78");
    themThe(lblDaHuy,      "Đã hủy",     "#B8503F");
    cards->addStretch();
    root->addLayout(cards);
    root->addSpacing(10);

    // ----- thanh lọc -----
    auto* bar = new QHBoxLayout();
    bar->addWidget(new QLabel("Hiển thị:", this));
    cbLoc = new QComboBox(this);
    cbLoc->addItem("Đang đi", (int)DANG_DI);
    cbLoc->addItem("Hoàn thành", (int)HOAN_THANH);
    cbLoc->addItem("Đã hủy", (int)DA_HUY);
    cbLoc->addItem("Tất cả", -1);
    bar->addWidget(cbLoc);
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
    table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->verticalHeader()->setVisible(false);
    root->addWidget(table, 1);

    lblTrong = new QLabel("Không có chuyến xe nào phù hợp.", this);
    lblTrong->setAlignment(Qt::AlignCenter);
    lblTrong->setStyleSheet("color:#96948C; padding:12px;");
    root->addWidget(lblTrong);

    connect(cbLoc, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]() { refresh(); });
    connect(btnLamMoi, &QPushButton::clicked, this, [this]() { refresh(); });

    // tự làm mới mỗi 10 giây, chỉ khi tab đang hiển thị
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() { if (isVisible()) refresh(); });
    timer->start(10000);

    refresh();
}

void TripPage::refresh() {
    heThong->taiXe().docFile();
    heThong->taxi().docFile();
    heThong->chuyenXe().docFile();

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
        bool homNayCoKhong = c.getThoiDiem().compare(0, 10, strHomNay) == 0;
        // chuyến hôm nay, hoặc chuyến còn "đang đi" từ ngày trước (taxi vẫn đang bị khóa)
        if (!homNayCoKhong && c.getTrangThai() != DANG_DI) continue;

        switch (c.getTrangThai()) {
        case DANG_DI:    ++demDangDi;    break;
        case HOAN_THANH: ++demHoanThanh; break;
        default:         ++demDaHuy;     break;
        }
        if (loc == -1 || (int)c.getTrangThai() == loc) hienThi.themCuoi(&c);
    }

    lblDangDi->setText(QString::number(demDangDi));
    lblHoanThanh->setText(QString::number(demHoanThanh));
    lblDaHuy->setText(QString::number(demDaHuy));

    table->setRowCount(hienThi.size());
    lblTrong->setVisible(hienThi.empty());
    table->setVisible(!hienThi.empty());

    for (int r = 0; r < hienThi.size(); ++r) {
        const ChuyenXe& c = *hienThi[r];

        // tra tài xế theo mã
        QString tenTX = "(đã xóa)", sdtTX = "-";
        for (auto& tx : heThong->taiXe().layDanhSach()) {
            if (tx.getMaTX() == c.getMaTaiXe()) {
                tenTX = QString::fromStdString(tx.getMaTX() + " - " + tx.getHoTen());
                sdtTX = QString::fromStdString(tx.getSoDienThoai());
                break;
            }
        }
        if (c.getMaTaiXe().empty()) tenTX = "(chưa gán)";

        QString lotrinh = QString::fromStdString(c.getDiemDon() + "  →  " + c.getDiemDen())
                          + QString("  (%1 km)").arg(c.getKhoangCach(), 0, 'f', 1);

        table->setItem(r, 0, new QTableWidgetItem(QString::fromStdString(c.getMaChuyen())));
        table->setItem(r, 1, new QTableWidgetItem(QString::fromStdString(c.getThoiDiem())));
        table->setItem(r, 2, new QTableWidgetItem(QString::fromStdString(c.getBienSoTaxi())));
        table->setItem(r, 3, new QTableWidgetItem(tenTX));
        table->setItem(r, 4, new QTableWidgetItem(sdtTX));
        table->setItem(r, 5, new QTableWidgetItem(
                                 QString::fromStdString(c.getTenKhachHang() + " - " + c.getSdtKhachHang())));
        table->setItem(r, 6, new QTableWidgetItem(lotrinh));
        table->setItem(r, 7, new QTableWidgetItem(QString::number(c.getCuocPhi(), 'f', 0) + " đ"));

        auto* itTT = new QTableWidgetItem(QString::fromStdString(trangThaiChuyenToStr(c.getTrangThai())));
        switch (c.getTrangThai()) {
        case DANG_DI:    itTT->setForeground(QColor("#4285C7")); break;
        case HOAN_THANH: itTT->setForeground(QColor("#438F78")); break;
        default:         itTT->setForeground(QColor("#B8503F")); break;
        }
        table->setItem(r, 8, itTT);
    }
}
