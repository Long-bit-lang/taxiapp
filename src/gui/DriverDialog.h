#ifndef DRIVERDIALOG_H
#define DRIVERDIALOG_H

#include <QDialog>
#include "../../include/TaiXe.h"

class QLineEdit;
class QComboBox;
class QDateEdit;

class DriverDialog : public QDialog {
    Q_OBJECT
public:
    explicit DriverDialog(QWidget* parent = nullptr, const TaiXe* existing = nullptr);
    TaiXe ketQua() const;

private:
    QLineEdit* txtMaTX;
    QLineEdit* txtTen;
    QLineEdit* txtHoDem;
    QDateEdit* dateNgaySinh;
    QLineEdit* txtDiaChi;
    QLineEdit* txtSdt;
    QLineEdit* txtBangLai;
    QLineEdit* txtGPLX;
    QComboBox* cbTrangThai;
};

#endif // DRIVERDIALOG_H
