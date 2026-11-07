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

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private slots:
    void onLoc();
    void onCellClicked(int row, int col);
    void hideOverlay();

private:
    HeThongTaxi* heThong;
    QComboBox* cbThang;
    QSpinBox* spinNam;
    QTableWidget* table;
    QLabel* lblTong;

    QLabel* overlayCell;
    int selectedRow = -1;
    int selectedCol = -1;

    void napDanhSachThang();
    void capNhatGioiHan();
};

#endif // STATSPAGE_H
