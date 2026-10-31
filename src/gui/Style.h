#ifndef STYLE_H
#define STYLE_H

#include <QString>
#include <QTableWidget>
#include <QHeaderView>

// Cài đặt chung cho mọi bảng: hàng cao, header cao, cột tự vừa nội dung,
// chỉ cột "cotGian" (và cotGian2 nếu có) chiếm phần thừa.
// Gọi SAU khi đã setColumnCount + setHorizontalHeaderLabels.
inline void caiDatBang(QTableWidget* t, int cotGian, int cotGian2 = -1) {
    auto* h = t->horizontalHeader();
    h->setSectionResizeMode(QHeaderView::Interactive);      // KHÔNG dùng ResizeToContents nữa
    h->setSectionResizeMode(cotGian, QHeaderView::Stretch);
    if (cotGian2 >= 0) h->setSectionResizeMode(cotGian2, QHeaderView::Stretch);
    h->setResizeContentsPrecision(100);                      // chỉ xét tối đa 100 dòng khi đo
    h->setFixedHeight(46);
    h->setMinimumSectionSize(90);
    h->setHighlightSections(false);

    t->verticalHeader()->setVisible(false);
    t->verticalHeader()->setDefaultSectionSize(44);
    t->setAlternatingRowColors(true);
    t->setShowGrid(false);
    t->setSelectionBehavior(QAbstractItemView::SelectRows);
    t->setEditTriggers(QAbstractItemView::NoEditTriggers);
    t->setTextElideMode(Qt::ElideRight);
}

inline void vuaNoiDung(QTableWidget* t) {
    auto* h = t->horizontalHeader();
    for (int c = 0; c < t->columnCount(); ++c)
        if (h->sectionResizeMode(c) != QHeaderView::Stretch)
            t->resizeColumnToContents(c);
}

inline QString appStyleSheet() {
    return R"(
        QWidget { font-family: "DejaVu Sans"; font-size: 15px; color: #3d3d3a; }
        QMainWindow, QWidget#page { background: #ffffff; }

        QFrame#titleBar { background: #4285C7; }
        QLabel#titleLabel { color: white; font-size: 19px; font-weight: bold; padding-left: 20px; }
        QLabel#pageTitle { font-size: 24px; font-weight: bold; color: #2C3E50; }

        QFrame#sidebar { background: #FAF9F6; border-right: 1px solid #E4E2D8; }
        QPushButton#menuBtn {
            text-align: left; padding: 12px 20px; border: none; background: transparent;
            border-radius: 8px; margin: 4px 12px; color: #3d3d3a; font-size: 15px;
        }
        QPushButton#menuBtn:checked { background: #DFEBF7; color: #2C6BA8; font-weight: bold; }
        QPushButton#menuBtn:hover:!checked { background: #EFEFEA; }

        QPushButton#btnPrimary {
            background: #4285C7; color: white; border: none; border-radius: 8px;
            padding: 10px 24px; font-weight: bold; font-size: 15px;
        }
        QPushButton#btnPrimary:hover { background: #386fa8; }

        QPushButton#btnSuccess {
            background: #438F78; color: white; border: none; border-radius: 8px;
            padding: 10px 24px; font-weight: bold; font-size: 15px;
        }
        QPushButton#btnSuccess:hover { background: #357a66; }

        QPushButton#btnFlat {
            background: white; color: #3d3d3a; border: 1px solid #96948C; border-radius: 8px;
            padding: 10px 24px; font-size: 15px;
        }
        QPushButton#btnFlat:hover { background: #F2F1EC; }

        QPushButton#btnDanger {
            background: white; color: #B8503F; border: 1px solid #B8503F; border-radius: 8px;
            padding: 10px 24px; font-size: 15px;
        }
        QPushButton#btnDanger:hover { background: #FBEDEA; }

        QTableWidget {
            border: 1px solid #C9C7BE; background: white; font-size: 15px;
            alternate-background-color: #F7F9FC;
            selection-background-color: #DFEBF7; selection-color: #2C6BA8;
        }
        QTableWidget::item { padding-left: 10px; padding-right: 10px; }
        QHeaderView::section {
            background: #DBE7F3; color: #2C3E50; font-weight: bold; font-size: 14px;
            padding: 8px 10px; border: none; border-bottom: 1px solid #96948C;
        }
        QLineEdit, QComboBox, QDateEdit, QSpinBox, QDoubleSpinBox {
            border: 1px solid #96948C; border-radius: 6px; padding: 8px 10px;
            background: white; min-height: 22px; font-size: 15px;
        }

        QSpinBox, QDoubleSpinBox { padding-right: 30px; }

        QSpinBox::up-button, QDoubleSpinBox::up-button {
            subcontrol-origin: border; subcontrol-position: top right;
            width: 24px; background: #F2F1EC;
            border-left: 1px solid #96948C; border-bottom: 1px solid #96948C;
            border-top-right-radius: 6px;
        }
        QSpinBox::down-button, QDoubleSpinBox::down-button {
            subcontrol-origin: border; subcontrol-position: bottom right;
            width: 24px; background: #F2F1EC;
            border-left: 1px solid #96948C;
            border-bottom-right-radius: 6px;
        }
        QSpinBox::up-button:hover, QDoubleSpinBox::up-button:hover,
        QSpinBox::down-button:hover, QDoubleSpinBox::down-button:hover { background: #DFEBF7; }

        QSpinBox::up-arrow, QDoubleSpinBox::up-arrow {
            image: url(:/icons/arrow_up.png); width: 10px; height: 6px;
        }
        QSpinBox::down-arrow, QDoubleSpinBox::down-arrow {
            image: url(:/icons/arrow_down.png); width: 10px; height: 6px;
        }

        QLineEdit:focus, QComboBox:focus { border: 1px solid #4285C7; }

        QFrame#card { background: #F2F1EC; border-radius: 10px; }
        QFrame#statCard { background: white; border: 1px solid #E4E2D8; border-radius: 10px; }

        QFrame#successBox { background: #D7EDE8; border: 2px solid #438F78; border-radius: 8px; }
        QFrame#taxiCard { background: white; border: 1px solid #96948C; border-radius: 6px; }
        QFrame#taxiCardSelected { background: #DFEBF7; border: 2px solid #4285C7; border-radius: 6px; }
    )";
}

#endif // STYLE_H
