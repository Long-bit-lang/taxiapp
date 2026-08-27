#include "DriverDialog.h"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QRegularExpression>

DriverDialog::DriverDialog(QWidget* parent, const TaiXe* existing)
    : QDialog(parent) {
    setWindowTitle(existing ? "Sua thong tin tai xe" : "Them tai xe moi");
    setMinimumWidth(380);

    auto* root = new QVBoxLayout(this);
    auto* form = new QFormLayout();
    form->setSpacing(10);

    txtMaTX = new QLineEdit(this);
    if (existing) { txtMaTX->setText(QString::fromStdString(existing->getMaTX())); }
    txtMaTX->setReadOnly(true);
    form->addRow("Ma tai xe:", txtMaTX);

    txtTen = new QLineEdit(this);
    txtHoDem = new QLineEdit(this);
    if (existing) {
        txtTen->setText(QString::fromStdString(existing->getTen()));
        txtHoDem->setText(QString::fromStdString(existing->getHoDem()));
    }
    form->addRow("Ten:", txtTen);
    form->addRow("Ho & dem:", txtHoDem);

    dateNgaySinh = new QDateEdit(this);
    dateNgaySinh->setDisplayFormat("dd/MM/yyyy");
    dateNgaySinh->setCalendarPopup(true);
    if (existing) {
        auto d = existing->getNgaySinh();
        dateNgaySinh->setDate(QDate(d.year, d.month, d.day));
    } else {
        dateNgaySinh->setDate(QDate(1995, 1, 1));
    }
    form->addRow("Ngay sinh:", dateNgaySinh);

    txtDiaChi = new QLineEdit(this);
    txtSdt = new QLineEdit(this);
    txtBangLai = new QLineEdit(this);
    txtGPLX = new QLineEdit(this);
    if (existing) {
        txtDiaChi->setText(QString::fromStdString(existing->getDiaChi()));
        txtSdt->setText(QString::fromStdString(existing->getSoDienThoai()));
        txtBangLai->setText(QString::fromStdString(existing->getBangLai()));
        txtGPLX->setText(QString::fromStdString(existing->getSoGPLX()));
    } else {
        txtBangLai->setText("B2");
    }
    form->addRow("Dia chi:", txtDiaChi);
    form->addRow("So dien thoai:", txtSdt);
    form->addRow("Bang lai:", txtBangLai);
    form->addRow("So GPLX:", txtGPLX);

    cbTrangThai = new QComboBox(this);
    cbTrangThai->addItems({"Dang hoat dong", "Nghi", "Ngung hoat dong"});
    if (existing) cbTrangThai->setCurrentIndex((int)existing->getTrangThai());
    form->addRow("Trang thai:", cbTrangThai);

    root->addLayout(form);

    auto* btnRow = new QHBoxLayout();
    auto* btnCancel = new QPushButton("Huy", this);
    btnCancel->setObjectName("btnFlat");
    auto* btnSave = new QPushButton("Luu", this);
    btnSave->setObjectName("btnPrimary");
    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(btnSave, &QPushButton::clicked, this, [this]() {
        if (txtTen->text().trimmed().isEmpty() || txtHoDem->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Loi", "Vui long nhap day du Ho & dem va Ten.");
            return;
        }
        if (txtDiaChi->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Loi", "Vui long nhap dia chi.");
            return;
        }

        static const QRegularExpression sdtRegex("^0[0-9]{9,10}$");
        QString sdt = txtSdt->text().trimmed();
        sdt.remove(' ');
        if (!sdtRegex.match(sdt).hasMatch()) {
            QMessageBox::warning(this, "Loi",
                "So dien thoai khong hop le (phai bat dau bang 0, gom 10 chu so).");
            return;
        }

        if (txtBangLai->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Loi", "Vui long nhap loai bang lai.");
            return;
        }
        if (txtGPLX->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Loi", "Vui long nhap so GPLX.");
            return;
        }

        QDate ns = dateNgaySinh->date();
        if (ns > QDate::currentDate()) {
            QMessageBox::warning(this, "Loi", "Ngay sinh khong the o tuong lai.");
            return;
        }
        int tuoi = ns.daysTo(QDate::currentDate()) / 365;
        if (tuoi < 18 || tuoi > 70) {
            QMessageBox::warning(this, "Loi", "Tai xe phai trong do tuoi tu 18 den 70.");
            return;
        }

        accept();
    });
    btnRow->addStretch();
    btnRow->addWidget(btnCancel);
    btnRow->addWidget(btnSave);
    root->addLayout(btnRow);
}

TaiXe DriverDialog::ketQua() const {
    Date d{ dateNgaySinh->date().day(), dateNgaySinh->date().month(), dateNgaySinh->date().year() };
    return TaiXe(
        txtMaTX->text().toStdString(),
        txtTen->text().toStdString(),
        txtHoDem->text().toStdString(),
        d,
        txtDiaChi->text().toStdString(),
        txtSdt->text().toStdString(),
        txtBangLai->text().toStdString(),
        txtGPLX->text().toStdString(),
        (TrangThai)cbTrangThai->currentIndex()
    );
}
