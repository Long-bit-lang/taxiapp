#ifndef STATSPAGE_H
#define STATSPAGE_H

#include <QWidget>
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
    void onChonTaxi(int index);

private:
    HeThongTaxi* heThong;
    QComboBox* cbTaxi;
    QTableWidget* table;
    QLabel* lblTong;
};

#endif // STATSPAGE_H
