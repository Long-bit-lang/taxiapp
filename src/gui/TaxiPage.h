#pragma once
#include <QWidget>
#include <QHash>
#include "../../include/HeThongTaxi.h"

class QTableWidget;
class QLineEdit;
class QComboBox;

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
    void onTimKiem();

private:
    HeThongTaxi* heThong;
    QTableWidget* table;
    QLineEdit* txtSearch;
    QComboBox* cbLocTrangThai;
    QHash<QString, QString> tenTheoMa;

    void updateDriverLookup();
    void fillTable();
};