#include "MainWindow.h"
#include "Style.h"
#include "DriverPage.h"
#include "TaxiPage.h"
#include "BookingPage.h"
#include "StatsPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>

MainWindow::MainWindow(HeThongTaxi* ht, std::shared_ptr<NguoiDung> nd, QWidget* parent)
    : QWidget(parent), heThong(ht), currentUser(nd) {

    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // title bar
    auto* titleBar = new QFrame(this);
    titleBar->setObjectName("titleBar");
    titleBar->setFixedHeight(46);
    auto* titleLay = new QHBoxLayout(titleBar);
    titleLay->setContentsMargins(0, 0, 0, 0);
    QString hello = QString("HE THONG QUAN LY TAXI TIEN SA — Xin chao, %1 (%2)")
                        .arg(QString::fromStdString(nd->getTenDangNhap()),
                             QString::fromStdString(nd->tenVaiTro()));
    auto* titleLbl = new QLabel(hello, titleBar);
    titleLbl->setObjectName("titleLabel");
    titleLay->addWidget(titleLbl);
    root->addWidget(titleBar);

    auto* body = new QHBoxLayout();
    body->setContentsMargins(0, 0, 0, 0);
    body->setSpacing(0);

    // sidebar
    auto* sidebar = new QFrame(this);
    sidebar->setObjectName("sidebar");
    sidebar->setFixedWidth(220);
    auto* sideLay = new QVBoxLayout(sidebar);
    sideLay->setContentsMargins(0, 14, 0, 14);
    sideLay->setSpacing(4);

    stack = new QStackedWidget(this);

    dashboardOverview = buildOverviewPage();
    stack->addWidget(dashboardOverview); // index 0

    bool laQuanLy = (nd->getVaiTro() == QUAN_LY);

    auto addMenuBtn = [&](const QString& text, int stackIndex) {
        auto* btn = new QPushButton(text, sidebar);
        btn->setObjectName("menuBtn");
        btn->setCheckable(true);
        btn->setMinimumHeight(38);
        connect(btn, &QPushButton::clicked, this, [this, stackIndex, btn]() { chuyenTrang(stackIndex, btn); });
        sideLay->addWidget(btn);
        menuButtons.push_back(btn);
        return btn;
    };

    QPushButton* btnOverview = addMenuBtn("Tong quan", 0);

    if (laQuanLy) {
        driverPage = new DriverPage(heThong, this);
        stack->addWidget(driverPage);
        addMenuBtn("Quan ly tai xe", stack->count() - 1);

        taxiPage = new TaxiPage(heThong, this);
        stack->addWidget(taxiPage);
        addMenuBtn("Quan ly taxi", stack->count() - 1);
    }

    bookingPage = new BookingPage(heThong, this);
    stack->addWidget(bookingPage);
    addMenuBtn("Dat chuyen xe", stack->count() - 1);

    if (laQuanLy) {
        statsPage = new StatsPage(heThong, this);
        stack->addWidget(statsPage);
        addMenuBtn("Thong ke / Bao cao", stack->count() - 1);
    }

    sideLay->addStretch();
    auto* btnLogout = new QPushButton("Dang xuat", sidebar);
    btnLogout->setObjectName("menuBtn");
    btnLogout->setMinimumHeight(38);
    connect(btnLogout, &QPushButton::clicked, this, [this]() { emit dangXuat(); });
    sideLay->addWidget(btnLogout);

    body->addWidget(sidebar);
    body->addWidget(stack, 1);
    root->addLayout(body, 1);

    chuyenTrang(0, btnOverview);
}

void MainWindow::chuyenTrang(int index, QPushButton* btnActive) {
    for (auto* b : menuButtons) b->setChecked(b == btnActive);
    stack->setCurrentIndex(index);
    if (stack->widget(index) == dashboardOverview) refreshOverview();
    if (driverPage && stack->widget(index) == driverPage) driverPage->refresh();
    if (taxiPage && stack->widget(index) == taxiPage) taxiPage->refresh();
    if (bookingPage && stack->widget(index) == bookingPage) bookingPage->refresh();
    if (statsPage && stack->widget(index) == statsPage) statsPage->refresh();
}

QWidget* MainWindow::buildOverviewPage() {
    auto* page = new QWidget(this);
    auto* lay = new QVBoxLayout(page);
    lay->setContentsMargins(20, 16, 20, 16);

    auto* lblTitle = new QLabel("Tong quan he thong", page);
    lblTitle->setStyleSheet("font-size:17px; font-weight:bold; color:#2C3E50;");
    lay->addWidget(lblTitle);
    lay->addSpacing(12);

    auto* grid = new QHBoxLayout();
    auto themThe = [&](QLabel*& outValueLabel, const QString& nhan) {
        auto* card = new QFrame(page);
        card->setObjectName("statCard");
        card->setFixedSize(150, 90);
        auto* cardLay = new QVBoxLayout(card);
        outValueLabel = new QLabel("0", card);
        outValueLabel->setAlignment(Qt::AlignCenter);
        outValueLabel->setStyleSheet("font-size:20px; font-weight:bold; color:#4285C7;");
        auto* lblNhan = new QLabel(nhan, card);
        lblNhan->setAlignment(Qt::AlignCenter);
        lblNhan->setStyleSheet("font-size:11px; color:#5a5a56;");
        cardLay->addWidget(outValueLabel);
        cardLay->addWidget(lblNhan);
        grid->addWidget(card);
    };

    themThe(lblSoTaiXe, "Tai xe");
    themThe(lblSoTaxi, "Taxi");
    themThe(lblSoChuyen, "Chuyen xe da ghi nhan");
    themThe(lblDoanhThu, "Tong doanh thu");

    grid->addStretch();
    lay->addLayout(grid);
    lay->addStretch();
    return page;
}

void MainWindow::refreshOverview() {
    heThong->taiXe().docFile();
    heThong->taxi().docFile();
    heThong->chuyenXe().docFile();
    int soTaiXe = (int)heThong->taiXe().layDanhSach().size();
    int soTaxi = (int)heThong->taxi().layDanhSach().size();
    int soChuyen = (int)heThong->chuyenXe().layDanhSach().size();
    double doanhThu = heThong->chuyenXe().tongDoanhThu();

    lblSoTaiXe->setText(QString::number(soTaiXe));
    lblSoTaxi->setText(QString::number(soTaxi));
    lblSoChuyen->setText(QString::number(soChuyen));
    lblDoanhThu->setText(QString::number(doanhThu / 1e6, 'f', 1) + "tr");
}
