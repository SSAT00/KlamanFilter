/********************************************************************************
** Form generated from reading UI file 'welcomewindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WELCOMEWINDOW_H
#define UI_WELCOMEWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WelcomeWindow
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *btnOpenWindowChooseGroup;
    QSpacerItem *horizontalSpacer_4;
    QFrame *fChooseSatellitesGroup;
    QGridLayout *gridLayout_3;
    QComboBox *cbStantions;
    QFrame *fStantions;
    QGridLayout *gridLayout_2;
    QTableWidget *twStantions;
    QComboBox *cbGroupNames;
    QSpacerItem *horizontalSpacer_8;
    QSpacerItem *horizontalSpacer;
    QFrame *fSatellites;
    QGridLayout *gridLayout_4;
    QToolBox *tlbSatellites;
    QWidget *page;
    QWidget *page_2;
    QSpacerItem *horizontalSpacer_6;
    QLabel *l1;
    QLabel *lGroupDisc;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer_5;
    QSpacerItem *horizontalSpacer_7;
    QLabel *lStantions;
    QCheckBox *cbNotUseNs;

    void setupUi(QWidget *WelcomeWindow)
    {
        if (WelcomeWindow->objectName().isEmpty())
            WelcomeWindow->setObjectName(QString::fromUtf8("WelcomeWindow"));
        WelcomeWindow->resize(1800, 1000);
        WelcomeWindow->setMinimumSize(QSize(1422, 870));
        WelcomeWindow->setMaximumSize(QSize(16777215, 16777215));
        QIcon icon;
        icon.addFile(QString::fromUtf8("img/IconSatellite.png"), QSize(), QIcon::Normal, QIcon::Off);
        WelcomeWindow->setWindowIcon(icon);
        gridLayout = new QGridLayout(WelcomeWindow);
        gridLayout->setSpacing(20);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetMinimumSize);
        gridLayout->setContentsMargins(20, 20, 20, 20);
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 1, 0, 1, 1);

        btnOpenWindowChooseGroup = new QPushButton(WelcomeWindow);
        btnOpenWindowChooseGroup->setObjectName(QString::fromUtf8("btnOpenWindowChooseGroup"));
        btnOpenWindowChooseGroup->setMinimumSize(QSize(150, 40));
        btnOpenWindowChooseGroup->setMaximumSize(QSize(150, 40));
        QFont font;
        font.setFamily(QString::fromUtf8("Montserrat Medium"));
        btnOpenWindowChooseGroup->setFont(font);
        btnOpenWindowChooseGroup->setToolTipDuration(0);
        btnOpenWindowChooseGroup->setStyleSheet(QString::fromUtf8(""));
        btnOpenWindowChooseGroup->setIconSize(QSize(100, 100));

        gridLayout->addWidget(btnOpenWindowChooseGroup, 1, 1, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_4, 1, 2, 1, 1);

        fChooseSatellitesGroup = new QFrame(WelcomeWindow);
        fChooseSatellitesGroup->setObjectName(QString::fromUtf8("fChooseSatellitesGroup"));
        fChooseSatellitesGroup->setEnabled(true);
        fChooseSatellitesGroup->setStyleSheet(QString::fromUtf8("QFrame#fChooseSatellitesGroup{\n"
"	border: 1px solid #D3D3D3;\n"
"	border-radius: 20px;\n"
"}"));
        fChooseSatellitesGroup->setFrameShape(QFrame::StyledPanel);
        fChooseSatellitesGroup->setFrameShadow(QFrame::Raised);
        gridLayout_3 = new QGridLayout(fChooseSatellitesGroup);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setHorizontalSpacing(10);
        gridLayout_3->setVerticalSpacing(5);
        gridLayout_3->setContentsMargins(20, 20, 20, 20);
        cbStantions = new QComboBox(fChooseSatellitesGroup);
        cbStantions->setObjectName(QString::fromUtf8("cbStantions"));
        cbStantions->setMinimumSize(QSize(300, 0));
        cbStantions->setMaximumSize(QSize(400, 16777215));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Montserrat"));
        font1.setPointSize(11);
        cbStantions->setFont(font1);

        gridLayout_3->addWidget(cbStantions, 2, 4, 1, 1);

        fStantions = new QFrame(fChooseSatellitesGroup);
        fStantions->setObjectName(QString::fromUtf8("fStantions"));
        fStantions->setMinimumSize(QSize(0, 600));
        fStantions->setStyleSheet(QString::fromUtf8("QFrame#fStantions{\n"
"	background-color:#fff;\n"
"	border:none;\n"
"	border-radius:20px;\n"
"	padding:20px;\n"
"}"));
        fStantions->setFrameShape(QFrame::StyledPanel);
        fStantions->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(fStantions);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        twStantions = new QTableWidget(fStantions);
        twStantions->setObjectName(QString::fromUtf8("twStantions"));
        twStantions->setMinimumSize(QSize(0, 500));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Montserrat"));
        font2.setPointSize(9);
        twStantions->setFont(font2);
        twStantions->setStyleSheet(QString::fromUtf8("QTableWidget#twStantions{\n"
"	border:none;\n"
"	border-radius: 20px;\n"
"}"));

        gridLayout_2->addWidget(twStantions, 0, 0, 1, 1);


        gridLayout_3->addWidget(fStantions, 4, 3, 1, 3);

        cbGroupNames = new QComboBox(fChooseSatellitesGroup);
        cbGroupNames->setObjectName(QString::fromUtf8("cbGroupNames"));
        cbGroupNames->setMinimumSize(QSize(300, 0));
        cbGroupNames->setMaximumSize(QSize(400, 16777215));
        cbGroupNames->setFont(font1);
        cbGroupNames->setMaxVisibleItems(10);

        gridLayout_3->addWidget(cbGroupNames, 2, 1, 1, 1);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_8, 2, 3, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 6, 4, 1, 2);

        fSatellites = new QFrame(fChooseSatellitesGroup);
        fSatellites->setObjectName(QString::fromUtf8("fSatellites"));
        fSatellites->setMinimumSize(QSize(0, 600));
        fSatellites->setStyleSheet(QString::fromUtf8("QFrame#fSatellites{\n"
"	background-color:#fff;\n"
"	border:none;\n"
"	border-radius:20px;\n"
"	padding:20px;\n"
"}"));
        fSatellites->setFrameShape(QFrame::StyledPanel);
        fSatellites->setFrameShadow(QFrame::Raised);
        gridLayout_4 = new QGridLayout(fSatellites);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        tlbSatellites = new QToolBox(fSatellites);
        tlbSatellites->setObjectName(QString::fromUtf8("tlbSatellites"));
        tlbSatellites->setMinimumSize(QSize(0, 500));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Montserrat"));
        tlbSatellites->setFont(font3);
        tlbSatellites->setStyleSheet(QString::fromUtf8("QToolBox#tlbSatellites{\n"
"	background-color: #fff;\n"
"}"));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        page->setGeometry(QRect(0, 0, 781, 614));
        tlbSatellites->addItem(page, QString::fromUtf8("Page 1"));
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        page_2->setGeometry(QRect(0, 0, 781, 614));
        tlbSatellites->addItem(page_2, QString::fromUtf8("Page 2"));

        gridLayout_4->addWidget(tlbSatellites, 0, 0, 1, 1);


        gridLayout_3->addWidget(fSatellites, 4, 0, 1, 3);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_6, 2, 0, 1, 1);

        l1 = new QLabel(fChooseSatellitesGroup);
        l1->setObjectName(QString::fromUtf8("l1"));
        QFont font4;
        font4.setFamily(QString::fromUtf8("Montserrat Medium"));
        font4.setPointSize(9);
        font4.setBold(false);
        font4.setWeight(50);
        l1->setFont(font4);
        l1->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(l1, 0, 0, 1, 6);

        lGroupDisc = new QLabel(fChooseSatellitesGroup);
        lGroupDisc->setObjectName(QString::fromUtf8("lGroupDisc"));
        lGroupDisc->setFont(font3);
        lGroupDisc->setStyleSheet(QString::fromUtf8("QLabel#lGroupDisc{\n"
"	padding-left:20px;\n"
"}"));

        gridLayout_3->addWidget(lGroupDisc, 3, 0, 1, 3);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_2, 6, 0, 1, 3);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_5, 2, 2, 1, 1);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_7, 2, 5, 1, 1);

        lStantions = new QLabel(fChooseSatellitesGroup);
        lStantions->setObjectName(QString::fromUtf8("lStantions"));
        lStantions->setFont(font3);
        lStantions->setStyleSheet(QString::fromUtf8("QLabel#lStantions{\n"
"	padding-left:20px;\n"
"}"));

        gridLayout_3->addWidget(lStantions, 3, 3, 1, 3);

        cbNotUseNs = new QCheckBox(fChooseSatellitesGroup);
        cbNotUseNs->setObjectName(QString::fromUtf8("cbNotUseNs"));
        cbNotUseNs->setFont(font3);
        cbNotUseNs->setStyleSheet(QString::fromUtf8("QCheckBox#cbNotUseNs{\n"
"	padding-left: 20px;\n"
"}"));

        gridLayout_3->addWidget(cbNotUseNs, 6, 3, 1, 1);


        gridLayout->addWidget(fChooseSatellitesGroup, 0, 0, 1, 3);


        retranslateUi(WelcomeWindow);

        tlbSatellites->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(WelcomeWindow);
    } // setupUi

    void retranslateUi(QWidget *WelcomeWindow)
    {
        WelcomeWindow->setWindowTitle(QCoreApplication::translate("WelcomeWindow", "\320\224\320\270\320\275\320\260\320\274\320\270\321\207\320\265\321\201\320\272\320\260\321\217 \321\200\320\265\320\272\321\203\321\200\321\201\320\270\320\262\320\275\320\260\321\217 \321\204\320\270\320\273\321\214\321\202\321\200\320\260\321\206\320\270\321\217 - \320\263\321\200\321\203\320\277\320\277\320\270\321\200\320\276\320\262\320\272\320\260", nullptr));
#if QT_CONFIG(tooltip)
        btnOpenWindowChooseGroup->setToolTip(QCoreApplication::translate("WelcomeWindow", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214 \321\201\320\276\321\205\321\200\320\260\320\275\320\265\320\275\320\275\321\203\321\216 \320\263\321\200\321\203\320\277\320\277\320\270\321\200\320\276\320\262\320\272\321\203", nullptr));
#endif // QT_CONFIG(tooltip)
        btnOpenWindowChooseGroup->setText(QCoreApplication::translate("WelcomeWindow", "\320\224\320\260\320\273\320\265\320\265", nullptr));
        tlbSatellites->setItemText(tlbSatellites->indexOf(page), QCoreApplication::translate("WelcomeWindow", "Page 1", nullptr));
        tlbSatellites->setItemText(tlbSatellites->indexOf(page_2), QCoreApplication::translate("WelcomeWindow", "Page 2", nullptr));
        l1->setText(QCoreApplication::translate("WelcomeWindow", "\320\222\321\213\320\261\320\276\321\200 \320\263\321\200\321\203\320\277\320\277\320\270\321\200\320\276\320\262\320\272\320\270", nullptr));
        lGroupDisc->setText(QCoreApplication::translate("WelcomeWindow", "\320\236\320\277\320\270\321\201\320\260\320\275\320\270\320\265 \320\263\321\200\321\203\320\277\320\277\320\270\321\200\320\276\320\262\320\272\320\270", nullptr));
        lStantions->setText(QCoreApplication::translate("WelcomeWindow", "\320\235\320\260\320\267\320\265\320\274\320\275\321\213\320\265 \321\201\321\202\320\260\320\275\321\206\320\270\320\270", nullptr));
        cbNotUseNs->setText(QCoreApplication::translate("WelcomeWindow", "\320\235\320\265 \320\270\321\201\320\277\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\321\214 \320\275\320\260\320\267\320\265\320\274\320\275\321\213\320\265 \321\201\321\202\320\260\320\275\321\206\320\270\320\270", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WelcomeWindow: public Ui_WelcomeWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WELCOMEWINDOW_H
