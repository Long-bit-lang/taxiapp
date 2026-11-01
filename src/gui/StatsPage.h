#ifndef STATSPAGE_H
#define STATSPAGE_H

#include <QWidget>
#include <QSpinBox>
#include "../../include/HeThongTaxi.h"

class QComboBox;
class QTableWidget;
class QLabel;

class StatsPage : public QWidget {
    Q_OBJECT
public:
    explicit StatsPage(HeThongTaxi* ht, QWidget* parent = nullptr);
    void refresh();

private slots:
    void onLoc();

private:
    HeThongTaxi* heThong;
    QComboBox* cbThang;
    QSpinBox* spinNam;
    QTableWidget* table;
    QLabel* lblTong;
    void napDanhSachThang();
    void capNhatGioiHan();
};

#endif // STATSPAGE_H
