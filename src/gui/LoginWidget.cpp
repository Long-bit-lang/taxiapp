#include "LoginWidget.h"
#include "Style.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFrame>

LoginWidget::LoginWidget(HeThongTaxi* ht, QWidget* parent)
    : QWidget(parent), heThong(ht) {

    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // title bar
    auto* titleBar = new QFrame(this);
    titleBar->setObjectName("titleBar");
    titleBar->setFixedHeight(46);
    auto* titleLay = new QHBoxLayout(titleBar);
    titleLay->setContentsMargins(0, 0, 0, 0);
    auto* titleLbl = new QLabel("HE THONG QUAN LY TAXI TIEN SA — DANG NHAP", titleBar);
    titleLbl->setObjectName("titleLabel");
    titleLay->addWidget(titleLbl);
    root->addWidget(titleBar);

    // center card
    auto* centerWrap = new QWidget(this);
    auto* centerLay = new QVBoxLayout(centerWrap);
    centerLay->setAlignment(Qt::AlignCenter);

    auto* card = new QFrame(centerWrap);
    card->setObjectName("card");
    card->setFixedWidth(360);
    auto* cardLay = new QVBoxLayout(card);
    cardLay->setContentsMargins(30, 26, 30, 26);
    cardLay->setSpacing(10);

    auto* lblTitle = new QLabel("DANG NHAP HE THONG", card);
    lblTitle->setStyleSheet("font-size:16px; font-weight:bold;");
    lblTitle->setAlignment(Qt::AlignCenter);
    cardLay->addWidget(lblTitle);
    cardLay->addSpacing(14);

    cardLay->addWidget(new QLabel("Ten dang nhap", card));
    txtTaiKhoan = new QLineEdit(card);
    txtTaiKhoan->setPlaceholderText("Username");
    cardLay->addWidget(txtTaiKhoan);

    cardLay->addSpacing(6);
    cardLay->addWidget(new QLabel("Mat khau", card));
    txtMatKhau = new QLineEdit(card);
    txtMatKhau->setEchoMode(QLineEdit::Password);
    txtMatKhau->setPlaceholderText("Password");
    cardLay->addWidget(txtMatKhau);

    lblLoi = new QLabel("", card);
    lblLoi->setStyleSheet("color:#B8503F; font-size:12px;");
    lblLoi->setAlignment(Qt::AlignCenter);
    cardLay->addWidget(lblLoi);

    cardLay->addSpacing(8);
    auto* btnLogin = new QPushButton("DANG NHAP", card);
    btnLogin->setObjectName("btnPrimary");
    btnLogin->setMinimumHeight(38);
    connect(btnLogin, &QPushButton::clicked, this, &LoginWidget::onDangNhap);
    connect(txtMatKhau, &QLineEdit::returnPressed, this, &LoginWidget::onDangNhap);
    cardLay->addWidget(btnLogin);

    centerLay->addWidget(card);
    root->addWidget(centerWrap, 1);

    setLayout(root);
}

void LoginWidget::onDangNhap() {
    std::string tk = txtTaiKhoan->text().toStdString();
    std::string mk = txtMatKhau->text().toStdString();
    auto nd = heThong->taiKhoan().kiemTraDangNhap(tk, mk);
    if (!nd) {
        lblLoi->setText("Sai ten dang nhap hoac mat khau!");
        return;
    }
    lblLoi->setText("");
    emit dangNhapThanhCong(std::shared_ptr<NguoiDung>(nd.release()));
}
