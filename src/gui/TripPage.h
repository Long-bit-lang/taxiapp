#ifndef TRIPPAGE_H
#define TRIPPAGE_H
#include <QWidget>
#include "../../include/HeThongTaxi.h"

class QTableWidget;
class QComboBox;
class QLabel;
class QTimer;

class TripPage : public QWidget {
    Q_OBJECT
public:
    explicit TripPage(HeThongTaxi* ht, QWidget* parent = nullptr);
    void refresh();

private:
    HeThongTaxi* heThong;
    QTableWidget* table;
    QComboBox* cbLoc;
    QLabel* lblNgay;
    QLabel* lblDangDi;
    QLabel* lblHoanThanh;
    QLabel* lblDaHuy;
    QLabel* lblTrong;
    QTimer* timer;
};
#endif // TRIPPAGE_H
