/********************************************************************************
** Form generated from reading UI file 'choosesatelliteswindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHOOSESATELLITESWINDOW_H
#define UI_CHOOSESATELLITESWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChooseSatellitesWindow
{
public:
    QGridLayout *gridLayout;
    QFrame *frame;
    QGridLayout *gridLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnNext;
    QToolBox *tlbChoosedSatellites;
    QWidget *page;
    QWidget *page_2;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QWidget *ChooseSatellitesWindow)
    {
        if (ChooseSatellitesWindow->objectName().isEmpty())
            ChooseSatellitesWindow->setObjectName(QString::fromUtf8("ChooseSatellitesWindow"));
        ChooseSatellitesWindow->resize(800, 600);
        ChooseSatellitesWindow->setMinimumSize(QSize(800, 600));
        QIcon icon;
        icon.addFile(QString::fromUtf8("img/IconSatellite.png"), QSize(), QIcon::Normal, QIcon::Off);
        ChooseSatellitesWindow->setWindowIcon(icon);
        gridLayout = new QGridLayout(ChooseSatellitesWindow);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(20, 20, 20, 20);
        frame = new QFrame(ChooseSatellitesWindow);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setStyleSheet(QString::fromUtf8("QFrame#frame{\n"
"	background-color:#fff;\n"
"	border:none;\n"
"	border-radius:20px;\n"
"	padding:20px;\n"
"}"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setVerticalSpacing(20);
        gridLayout_2->setContentsMargins(20, 20, 20, 20);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 1, 0, 1, 1);

        btnNext = new QPushButton(frame);
        btnNext->setObjectName(QString::fromUtf8("btnNext"));
        btnNext->setMinimumSize(QSize(140, 40));
        btnNext->setMaximumSize(QSize(140, 40));
        QFont font;
        font.setFamily(QString::fromUtf8("Montserrat"));
        btnNext->setFont(font);

        gridLayout_2->addWidget(btnNext, 1, 1, 1, 1);

        tlbChoosedSatellites = new QToolBox(frame);
        tlbChoosedSatellites->setObjectName(QString::fromUtf8("tlbChoosedSatellites"));
        tlbChoosedSatellites->setFont(font);
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        page->setGeometry(QRect(0, 0, 680, 352));
        tlbChoosedSatellites->addItem(page, QString::fromUtf8("Page 1"));
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        page_2->setGeometry(QRect(0, 0, 680, 352));
        tlbChoosedSatellites->addItem(page_2, QString::fromUtf8("Page 2"));

        gridLayout_2->addWidget(tlbChoosedSatellites, 0, 0, 1, 3);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 1, 2, 1, 1);


        gridLayout->addWidget(frame, 0, 0, 1, 1);


        retranslateUi(ChooseSatellitesWindow);

        tlbChoosedSatellites->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ChooseSatellitesWindow);
    } // setupUi

    void retranslateUi(QWidget *ChooseSatellitesWindow)
    {
        ChooseSatellitesWindow->setWindowTitle(QCoreApplication::translate("ChooseSatellitesWindow", "\320\222\321\213\320\261\320\276\321\200 \321\201\320\277\321\203\321\202\320\275\320\270\320\272\320\276\320\262", nullptr));
        btnNext->setText(QCoreApplication::translate("ChooseSatellitesWindow", "\320\224\320\260\320\273\320\265\320\265", nullptr));
        tlbChoosedSatellites->setItemText(tlbChoosedSatellites->indexOf(page), QCoreApplication::translate("ChooseSatellitesWindow", "Page 1", nullptr));
        tlbChoosedSatellites->setItemText(tlbChoosedSatellites->indexOf(page_2), QCoreApplication::translate("ChooseSatellitesWindow", "Page 2", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChooseSatellitesWindow: public Ui_ChooseSatellitesWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHOOSESATELLITESWINDOW_H
