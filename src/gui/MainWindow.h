#pragma once
#include <QWidget>
#include "../../include/HeThongTaxi.h"
#include "../../include/NguoiDung.h"

class QStackedWidget;
class QPushButton;
class QLabel;
class DriverPage;
class TaxiPage;
class BookingPage;
class StatsPage;
class TripPage;

class MainWindow : public QWidget {
    Q_OBJECT
public:
    explicit MainWindow(HeThongTaxi* ht, std::shared_ptr<NguoiDung> nguoiDung, QWidget* parent = nullptr);

signals:
    void dangXuat();

private:
    HeThongTaxi* heThong;
    std::shared_ptr<NguoiDung> currentUser;
    QStackedWidget* stack;
    QWidget* dashboardOverview;
    DriverPage* driverPage = nullptr;
    TaxiPage* taxiPage = nullptr;
    BookingPage* bookingPage = nullptr;
    StatsPage* statsPage = nullptr;
    TripPage* tripPage = nullptr;
    DanhSach<QPushButton*> menuButtons;

    QLabel* lblSoTaiXe = nullptr;
    QLabel* lblSoTaxi = nullptr;
    QLabel* lblSoChuyen = nullptr;
    QLabel* lblDoanhThu = nullptr;
    void refreshOverview();

    QWidget* buildOverviewPage();
    void chuyenTrang(int index, QPushButton* btnActive);
};