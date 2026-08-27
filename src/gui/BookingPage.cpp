#include "BookingPage.h"
#include "Style.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QRegularExpression> 
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QMessageBox>
#include <QDateTime>
#include <QButtonGroup>
#include <QRadioButton>

BookingPage::BookingPage(HeThongTaxi* ht, QWidget* parent)
    : QWidget(parent), heThong(ht) {

    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(20, 16, 20, 16);

    auto* lblTitle = new QLabel("DAT CHUYEN XE", this);
    lblTitle->setStyleSheet("font-size:17px; font-weight:bold; color:#2C3E50;");
    root->addWidget(lblTitle);
    root->addSpacing(10);

    auto* row = new QHBoxLayout();

    // left card: customer info form
    auto* leftCard = new QFrame(this);
    leftCard->setObjectName("card");
    auto* leftLay = new QVBoxLayout(leftCard);
    leftLay->addWidget(new QLabel("<b>Thong tin khach hang</b>", leftCard));

    auto* form = new QFormLayout();
    txtTenKhach = new QLineEdit(leftCard);
    txtSdt = new QLineEdit(leftCard);
    txtDiemDon = new QLineEdit(leftCard);
    txtDiemDen = new QLineEdit(leftCard);

    spinKhoangCach = new QDoubleSpinBox(leftCard);
    spinKhoangCach->setRange(0.1, 200);
    spinKhoangCach->setValue(12.5);
    spinKhoangCach->setSuffix(" km");

    spinSoKhach = new QSpinBox(leftCard);
    spinSoKhach->setRange(1, 16);
    spinSoKhach->setValue(1);
    spinSoKhach->setSuffix(" khach");
    connect(spinSoKhach, QOverload<int>::of(&QSpinBox::valueChanged), this, &BookingPage::onTimTaxi);

    form->addRow("Ho ten khach hang:", txtTenKhach);
    form->addRow("So dien thoai:", txtSdt);
    form->addRow("Diem don:", txtDiemDon);
    form->addRow("Diem den:", txtDiemDen);
    form->addRow("Khoang cach:", spinKhoangCach);
    form->addRow("So khach:", spinSoKhach);
    leftLay->addLayout(form);

    auto* btnTim = new QPushButton("TIM TAXI PHU HOP", leftCard);
    btnTim->setObjectName("btnPrimary");
    btnTim->setMinimumHeight(38);
    connect(btnTim, &QPushButton::clicked, this, &BookingPage::onTimTaxi);
    leftLay->addSpacing(6);
    leftLay->addWidget(btnTim);
    leftLay->addStretch();

    // right card: suggested taxi list
    auto* rightCard = new QFrame(this);
    rightCard->setObjectName("card");
    auto* rightLay = new QVBoxLayout(rightCard);
    rightLay->addWidget(new QLabel("<b>Danh sach taxi de xuat (uu tien thoi gian don som nhat)</b>", rightCard));

    dsTaxiContainer = new QWidget(rightCard);
    dsTaxiLayout = new QVBoxLayout(dsTaxiContainer);
    dsTaxiLayout->setSpacing(8);
    rightLay->addWidget(dsTaxiContainer);
    rightLay->addStretch();

    rightLay->addWidget(new QLabel("Chon tai xe thuc hien:", rightCard));
    cbTaiXe = new QComboBox(rightCard);
    rightLay->addWidget(cbTaiXe);
    
    btnXacNhan = new QPushButton("XAC NHAN DAT XE", rightCard);
    btnXacNhan->setObjectName("btnSuccess");
    btnXacNhan->setMinimumHeight(38);
    btnXacNhan->setEnabled(false);
    connect(btnXacNhan, &QPushButton::clicked, this, &BookingPage::onXacNhan);
    rightLay->addWidget(btnXacNhan);

    row->addWidget(leftCard, 1);
    row->addWidget(rightCard, 1);
    root->addLayout(row);

    hienThiDanhSachTaxi();
}

void BookingPage::hienThiDanhSachTaxi() {
    QLayoutItem* item;
    while ((item = dsTaxiLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    bienSoDangChon.clear();
    btnXacNhan->setEnabled(false);
    cbTaiXe->clear();

    heThong->taxi().docFile();
    heThong->taiXe().docFile();

    auto ds = heThong->taxi().timTaxiPhuHop(spinSoKhach->value());
    if (ds.empty()) {
        dsTaxiLayout->addWidget(new QLabel(
            "Khong co taxi nao san sang va du cho cho so khach da nhap.", dsTaxiContainer));
        return;
    }
    bool firstSet = false;
    for (auto* t : ds) {
        QString bienSo = QString::fromStdString(t->getBienSo());
        auto* card = new QPushButton(dsTaxiContainer);
        card->setCheckable(true);
        card->setCursor(Qt::PointingHandCursor);
        card->setMinimumHeight(56);
        card->setProperty("bienSo", bienSo);
        card->setText(QString("Bien so: %1   |   %2 cho\nThoi gian don du kien: ~3-8 phut")
                      .arg(bienSo).arg(t->getSucChua()));
        card->setStyleSheet(
            "QPushButton { text-align: left; padding: 10px 14px; border: 1px solid #96948C;"
            " border-radius: 6px; background: white; }"
            "QPushButton:checked { border: 2px solid #4285C7; background: #DFEBF7; font-weight: bold; }"
        );
        connect(card, &QPushButton::clicked, this, [this, bienSo]() { chonTaxi(bienSo); });
        dsTaxiLayout->addWidget(card);

        if (!firstSet) { chonTaxi(bienSo); firstSet = true; }
    }
}

void BookingPage::refresh() {
    hienThiDanhSachTaxi();
}

void BookingPage::chonTaxi(const QString& bienSo) {
    bienSoDangChon = bienSo;
    btnXacNhan->setEnabled(true);
    cbTaiXe->clear();
    for (int i = 0; i < dsTaxiLayout->count(); ++i) {
        auto* w = qobject_cast<QPushButton*>(dsTaxiLayout->itemAt(i)->widget());
        if (!w) continue;
        bool sel = (w->property("bienSo").toString() == bienSo);
        w->setChecked(sel);
    }
    
    Taxi* taxi = heThong->taxi().timTheoBienSo(bienSo.toStdString());
    if (taxi) {
        for (auto& maTX : taxi->getDsMaTaiXe()) {
            for (auto& tx : heThong->taiXe().layDanhSach()) {
                if (tx.getMaTX() == maTX && tx.getTrangThai() == DANG_HOAT_DONG && !heThong->chuyenXe().taiXeDangChay(maTX)) {
                    cbTaiXe->addItem(QString::fromStdString(tx.getHoTen()),
                                      QString::fromStdString(maTX));
                }
            }
        }
    }
    if (cbTaiXe->count() == 0) {
        cbTaiXe->addItem("(Chua co tai xe phu hop)", "");
        btnXacNhan->setEnabled(false);
    } else {
        btnXacNhan->setEnabled(true);
    }
}

void BookingPage::onTimTaxi() {
    hienThiDanhSachTaxi();
}

void BookingPage::onXacNhan() {
    if (bienSoDangChon.isEmpty()) return;
    if (txtTenKhach->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Loi", "Vui long nhap ho ten khach hang.");
        return;
    }
    static const QRegularExpression sdtRegex("^0[0-9]{9,10}$");
    QString sdtKhach = txtSdt->text();
    sdtKhach.remove(' ');
    if (!sdtRegex.match(sdtKhach).hasMatch()) {
        QMessageBox::warning(this, "Loi", "So dien thoai khach hang khong hop le.");
        return;
    }
    if (txtDiemDon->text().trimmed().isEmpty() || txtDiemDen->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Loi", "Vui long nhap day du diem don va diem den.");
        return;
    }
    Taxi* taxi = heThong->taxi().timTheoBienSo(bienSoDangChon.toStdString());
    if (!taxi) return;
    
    QString maTaiXeChon = cbTaiXe->currentData().toString();
    if (maTaiXeChon.isEmpty()) {
        QMessageBox::warning(this, "Loi", "Taxi nay chua co tai xe phu hop de thuc hien chuyen.");
        return;
    }

    if (heThong->chuyenXe().taiXeDangChay(maTaiXeChon.toStdString())) {
        QMessageBox::warning(this, "Loi",
                             "Tai xe nay hien dang thuc hien mot chuyen xe khac, vui long chon tai xe khac.");
        chonTaxi(bienSoDangChon);
        return;
    }

    if (spinSoKhach->value() > taxi->getSucChua()) {
        QMessageBox::warning(this, "Loi",
                             QString("Taxi %1 chi cho toi da %2 khach, khong du cho %3 khach da nhap.")
                                 .arg(bienSoDangChon).arg(taxi->getSucChua()).arg(spinSoKhach->value()));
        hienThiDanhSachTaxi();  // lam moi lai danh sach cho khop
        return;
    }

    std::string maChuyen = heThong->chuyenXe().sinhMaMoi();
    std::string thoiDiem = QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm").toStdString();
    ChuyenXe cx(maChuyen, txtTenKhach->text().toStdString(), txtSdt->text().toStdString(),
                bienSoDangChon.toStdString(), thoiDiem, txtDiemDon->text().toStdString(),
                txtDiemDen->text().toStdString(), spinKhoangCach->value(), maTaiXeChon.toStdString());

    heThong->chuyenXe().themChuyen(cx);
    taxi->setSanSang(false);
    heThong->taxi().ghiFile();

    QMessageBox box(this);
    box.setWindowTitle("Thanh cong");
    box.setIcon(QMessageBox::Information);
    box.setText(QString("DAT CHUYEN XE THANH CONG\n\nMa chuyen xe: %1\nTaxi: %2\nCuoc phi du kien: %3 d")
                .arg(QString::fromStdString(maChuyen), bienSoDangChon)
                .arg(QString::number(cx.getCuocPhi(), 'f', 0)));
    box.exec();

    hienThiDanhSachTaxi();
}
