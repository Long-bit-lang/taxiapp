QT       += core gui widgets
CONFIG   += c++17
TARGET    = TaxiTienSa
TEMPLATE  = app

SOURCES += \
    src/main.cpp \
    src/gui/MainWindow.cpp \
    src/gui/LoginWidget.cpp \
    src/gui/DriverPage.cpp \
    src/gui/DriverDialog.cpp \
    src/gui/TaxiPage.cpp \
    src/gui/BookingPage.cpp \
    src/gui/StatsPage.cpp

HEADERS += \
    src/gui/MainWindow.h \
    src/gui/LoginWidget.h \
    src/gui/DriverPage.h \
    src/gui/DriverDialog.h \
    src/gui/TaxiPage.h \
    src/gui/BookingPage.h \
    src/gui/StatsPage.h \
    src/gui/Style.h \
    include/HeThongTaxi.h \
    include/QuanLyTaiXe.h \
    include/QuanLyTaxi.h \
    include/QuanLyChuyenXe.h \
    include/QuanLyTaiKhoan.h \
    include/TaiXe.h \
    include/Taxi.h \
    include/ChuyenXe.h \
    include/NguoiDung.h \
    include/Common.h
