#ifndef TAXIPAGE_H
#define TAXIPAGE_H

#include <QWidget>
#include "../../include/HeThongTaxi.h"

class QTableWidget;

class TaxiPage : public QWidget {
    Q_OBJECT
public:
    explicit TaxiPage(HeThongTaxi* ht, QWidget* parent = nullptr);
    void refresh();

private slots:
    void onThem();
    void onXoa();
    void onPhanCong();
    void onHoanThanh();
    void onHuyChuyen();

private:
    HeThongTaxi* heThong;
    QTableWidget* table;
};

#endif // TAXIPAGE_H
