#pragma once
#include <QWidget>
#include "../../include/HeThongTaxi.h"

class QTableWidget;
class QComboBox;
class QLabel;
class QTimer;
class QFrame;
class QPushButton;

class TripPage : public QWidget {
    Q_OBJECT
public:
    explicit TripPage(HeThongTaxi* ht, QWidget* parent = nullptr);
    void refresh(bool reloadFromDisk = false);

signals:
    void chuyenXeThayDoi();

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private slots:
    void onCellClicked(int row, int col);
    void hideOverlay();
    void onHoanThanh();
    void onHuyChuyen();

private:
    HeThongTaxi* heThong;
    QTableWidget* table;
    QComboBox* cbLoc;
    QLabel* lblNgay;
    QLabel* lblDangDi;
    QLabel* lblHoanThanh;
    QLabel* lblDaHuy;
    QFrame* cardDangDi = nullptr;
    QFrame* cardHoanThanh = nullptr;
    QFrame* cardDaHuy = nullptr;
    QPushButton* btnHoanThanh = nullptr;
    QPushButton* btnHuyChuyen = nullptr;
    QLabel* lblTrong;
    QTimer* timer;

    QLabel* overlayCell;
    int selectedRow = -1;
    int selectedCol = -1;

    void capNhatTrangThaiNut();
    void capNhatVienTheLoc();
};
