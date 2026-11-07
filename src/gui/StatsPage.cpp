#include "Style.h"
#include "StatsPage.h"
#include <QSpinBox>
#include "../../include/TinhLuong.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QFrame>
#include <QDate>
#include <QSignalBlocker>
#include <QScrollBar>
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <QKeyEvent>

StatsPage::StatsPage(HeThongTaxi* ht, QWidget* parent) : QWidget(parent), heThong(ht) {
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(20, 16, 20, 16);

    auto* lblTitle = new QLabel("THỐNG KÊ / BÁO CÁO — Bảng lương tài xế", this);
    lblTitle->setObjectName("pageTitle");
    root->addWidget(lblTitle);
    root->addSpacing(8);

    auto* row = new QHBoxLayout();
    row->addWidget(new QLabel("Năm:", this));
    spinNam = new QSpinBox(this);
    row->addWidget(spinNam);
    row->addSpacing(12);
    row->addWidget(new QLabel("Tháng:", this));
    cbThang = new QComboBox(this);
    row->addWidget(cbThang);
    row->addStretch();
    root->addLayout(row);

    spinNam->setRange(2020, QDate::currentDate().year());
    spinNam->setValue(QDate::currentDate().year());
    napDanhSachThang();

    connect(cbThang, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &StatsPage::onLoc);
    connect(spinNam, QOverload<int>::of(&QSpinBox::valueChanged), this, [this]() {
        napDanhSachThang();   // đổi năm thì nạp lại danh sách tháng
        onLoc();
    });

    table = new QTableWidget(this);
    table->setColumnCount(9);
    table->setHorizontalHeaderLabels({"Mã TX", "Họ và tên", "Số chuyến", "Tổng km",
                                      "Doanh thu", "Chiết khấu", "Phí nền tảng", "Thưởng", "Thực nhận"});
    caiDatBang(table, 1);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->verticalHeader()->setVisible(false);
    root->addWidget(table, 1);

    // Nhãn nổi mở rộng hiển thị trọn vẹn thông tin khi ô bị cắt "..."
    overlayCell = new QLabel(table->viewport());
    overlayCell->setObjectName("cellDetailOverlay");
    overlayCell->setWordWrap(true);
    overlayCell->setTextInteractionFlags(Qt::TextSelectableByMouse);
    overlayCell->setStyleSheet(
        "QLabel#cellDetailOverlay {"
        "  background: #FFFFFF;"
        "  color: #2C3E50;"
        "  font-size: 14px;"
        "  border: 2px solid #4285C7;"
        "  border-radius: 6px;"
        "  padding: 6px 12px;"
        "}"
    );
    auto* shadow = new QGraphicsDropShadowEffect(overlayCell);
    shadow->setBlurRadius(12);
    shadow->setColor(QColor(0, 0, 0, 70));
    shadow->setOffset(0, 3);
    overlayCell->setGraphicsEffect(shadow);
    overlayCell->hide();

    connect(table, &QTableWidget::cellClicked, this, &StatsPage::onCellClicked);
    connect(table->verticalScrollBar(), &QScrollBar::valueChanged, this, &StatsPage::hideOverlay);
    connect(table->horizontalScrollBar(), &QScrollBar::valueChanged, this, &StatsPage::hideOverlay);
    table->viewport()->installEventFilter(this);

    auto* totalCard = new QFrame(this);
    totalCard->setObjectName("card");
    auto* totalLay = new QHBoxLayout(totalCard);
    totalLay->addWidget(new QLabel("<b>TỔNG CỘNG</b>", totalCard));
    totalLay->addStretch();
    lblTong = new QLabel("0 đ", totalCard);
    lblTong->setStyleSheet("font-size:20px; font-weight:bold; color:#4285C7;");
    totalLay->addWidget(lblTong);
    root->addWidget(totalCard);

    refresh();
}

bool StatsPage::eventFilter(QObject* watched, QEvent* event) {
    if (watched == table->viewport()) {
        if (event->type() == QEvent::MouseButtonPress) {
            auto* me = static_cast<QMouseEvent*>(event);
            QModelIndex idx = table->indexAt(me->pos());
            if (!idx.isValid()) {
                hideOverlay();
            }
        }
    }
    if (event->type() == QEvent::KeyPress) {
        auto* ke = static_cast<QKeyEvent*>(event);
        if (ke->key() == Qt::Key_Escape) {
            hideOverlay();
        }
    }
    return QWidget::eventFilter(watched, event);
}

void StatsPage::hideOverlay() {
    selectedRow = -1;
    selectedCol = -1;
    if (overlayCell) overlayCell->hide();
}

void StatsPage::onCellClicked(int row, int col) {
    auto* it = table->item(row, col);
    if (!it || it->text().isEmpty()) {
        hideOverlay();
        return;
    }

    // Nếu bấm lại chính ô đang mở -> thu gọn lại
    if (selectedRow == row && selectedCol == col && overlayCell && overlayCell->isVisible()) {
        hideOverlay();
        return;
    }

    // Đóng ô cũ trước (ô cũ trở lại bình thường)
    hideOverlay();

    QRect cellRect = table->visualItemRect(it);
    if (cellRect.isNull() || !cellRect.isValid()) return;

    QFontMetrics fm(table->font());
    int textWidth = fm.horizontalAdvance(it->text());

    // Kiểm tra xem nội dung ô có bị tràn (hiện "...") hay không
    bool isTruncated = (textWidth > cellRect.width() - 20) || it->text().contains('\n');

    if (isTruncated) {
        selectedRow = row;
        selectedCol = col;

        overlayCell->setText(it->text());

        int targetW = qMax(cellRect.width(), qMin(textWidth + 28, 550));
        overlayCell->setFixedWidth(targetW);
        overlayCell->adjustSize();

        int x = cellRect.x();
        int y = cellRect.y();

        if (x + overlayCell->width() > table->viewport()->width()) {
            x = qMax(0, table->viewport()->width() - overlayCell->width() - 6);
        }
        if (y + overlayCell->height() > table->viewport()->height()) {
            y = qMax(0, cellRect.bottom() - overlayCell->height());
        }

        overlayCell->move(x, y);
        overlayCell->show();
        overlayCell->raise();
    }
}

void StatsPage::capNhatGioiHan() {
    QSignalBlocker chan(spinNam);
    spinNam->setRange(2020, QDate::currentDate().year());   // tự kẹp giá trị nếu vượt giới hạn
}

void StatsPage::napDanhSachThang() {
    QDate homNay = QDate::currentDate();
    int thangToiDa = (spinNam->value() >= homNay.year()) ? homNay.month() : 12;
    int dangChon = cbThang->currentIndex() >= 0 ? cbThang->currentData().toInt() : 0;

    QSignalBlocker chan(cbThang);
    cbThang->clear();
    cbThang->addItem("Cả năm", 0);
    for (int m = 1; m <= thangToiDa; ++m)
        cbThang->addItem(QString("Tháng %1").arg(m), m);

    int idx = cbThang->findData(dangChon);
    cbThang->setCurrentIndex(idx >= 0 ? idx : 0);   // tháng cũ không còn hợp lệ thì về "Cả năm"
}

void StatsPage::refresh() {
    hideOverlay();

    heThong->taiXe().docFile();
    heThong->taxi().docFile();
    heThong->chuyenXe().docFile();
    capNhatGioiHan();
    napDanhSachThang();
    onLoc();
}

void StatsPage::onLoc() {
    hideOverlay();

    auto ds = tinhBangLuong(heThong->chuyenXe(), heThong->taxi(),
                            cbThang->currentData().toInt(), spinNam->value());

    // Nạp sẵn bản đồ tên tài xế O(M)
    QHash<QString, QString> tenTheoMa;
    for (const auto& tx : heThong->taiXe().layDanhSach()) {
        tenTheoMa.insert(QString::fromStdString(tx.getMaTX()), QString::fromStdString(tx.getHoTen()));
    }

    table->setUpdatesEnabled(false);
    table->setRowCount((int)ds.size());
    double tongThucNhan = 0;

    for (int i = 0; i < (int)ds.size(); ++i) {
        const auto& l = ds[i];
        QString hoTen = tenTheoMa.value(QString::fromStdString(l.maTX), "(đã xóa)");

        auto setItem = [this, i](int col, const QString& text, const QColor& fg = QColor()) {
            auto* it = table->item(i, col);
            if (!it) {
                it = new QTableWidgetItem(text);
                table->setItem(i, col, it);
            } else {
                it->setText(text);
            }
            it->setToolTip(text);
            if (fg.isValid()) it->setForeground(fg);
            else it->setForeground(QColor("#3d3d3a"));
        };

        setItem(0, QString::fromStdString(l.maTX));
        setItem(1, hoTen);
        setItem(2, QString::number(l.soChuyen));
        setItem(3, QString::number(l.tongKm, 'f', 1) + " km");
        setItem(4, dinhDangTien(l.doanhThu));
        setItem(5, dinhDangTien(-l.chietKhau));
        setItem(6, dinhDangTien(-l.phiNenTang));
        setItem(7, dinhDangTien(l.thuong));
        setItem(8, dinhDangTien(l.thucNhan), QColor("#438F78"));

        tongThucNhan += l.thucNhan;
    }

    lblTong->setText(dinhDangTien(tongThucNhan));
    table->setUpdatesEnabled(true);
    vuaNoiDung(table);
}
