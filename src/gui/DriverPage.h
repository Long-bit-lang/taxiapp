#pragma once
#include <QWidget>
#include "../../include/HeThongTaxi.h"

class QTableWidget;
class QLineEdit;

class DriverPage : public QWidget {
    Q_OBJECT
public:
    explicit DriverPage(HeThongTaxi* ht, QWidget* parent = nullptr);
    void refresh();

signals:
    void taiXeThayDoi();

private slots:
    void onThem();
    void onSua();
    void onXoa();
    void onTimKiem(const QString& tuKhoa);

private:
    HeThongTaxi* heThong;
    QTableWidget* table;
    QLineEdit* txtSearch;
    void fillTable(const DanhSach<TaiXe>& ds);
};
