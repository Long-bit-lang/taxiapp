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

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private slots:
    void onCellClicked(int row, int col);
    void hideOverlay();

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

    QLabel* overlayCell;
    int selectedRow = -1;
    int selectedCol = -1;
};

#endif // TRIPPAGE_H
