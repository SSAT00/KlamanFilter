/********************************************************************************
** Form generated from reading UI file 'groupvisibilitywindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GROUPVISIBILITYWINDOW_H
#define UI_GROUPVISIBILITYWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GroupVisibilityWindow
{
public:
    QGridLayout *gridLayout;
    QPushButton *btnNext;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_2;
    QFrame *frame;
    QGridLayout *gridLayout_3;
    QFrame *fSat;

    void setupUi(QWidget *GroupVisibilityWindow)
    {
        if (GroupVisibilityWindow->objectName().isEmpty())
            GroupVisibilityWindow->setObjectName(QString::fromUtf8("GroupVisibilityWindow"));
        GroupVisibilityWindow->resize(1200, 1024);
        GroupVisibilityWindow->setMinimumSize(QSize(1200, 1024));
        QFont font;
        font.setPointSize(8);
        GroupVisibilityWindow->setFont(font);
        QIcon icon;
        icon.addFile(QString::fromUtf8("img/IconSatellite.png"), QSize(), QIcon::Normal, QIcon::Off);
        GroupVisibilityWindow->setWindowIcon(icon);
        gridLayout = new QGridLayout(GroupVisibilityWindow);
        gridLayout->setSpacing(10);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(20, 20, 20, 20);
        btnNext = new QPushButton(GroupVisibilityWindow);
        btnNext->setObjectName(QString::fromUtf8("btnNext"));
        btnNext->setMinimumSize(QSize(140, 40));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Montserrat"));
        btnNext->setFont(font1);

        gridLayout->addWidget(btnNext, 1, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 2, 1, 1);

        frame = new QFrame(GroupVisibilityWindow);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_3 = new QGridLayout(frame);
        gridLayout_3->setSpacing(10);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        fSat = new QFrame(frame);
        fSat->setObjectName(QString::fromUtf8("fSat"));
        fSat->setMaximumSize(QSize(16777215, 16777215));
        fSat->setFont(font1);
        fSat->setStyleSheet(QString::fromUtf8("QFrame#fSat{\n"
"	background-color:#fff;\n"
"	border:none;\n"
"	border-radius: 20px;\n"
"}"));
        fSat->setFrameShape(QFrame::StyledPanel);
        fSat->setFrameShadow(QFrame::Raised);

        gridLayout_3->addWidget(fSat, 0, 0, 1, 1);


        gridLayout->addWidget(frame, 0, 0, 1, 3);


        retranslateUi(GroupVisibilityWindow);

        QMetaObject::connectSlotsByName(GroupVisibilityWindow);
    } // setupUi

    void retranslateUi(QWidget *GroupVisibilityWindow)
    {
        GroupVisibilityWindow->setWindowTitle(QCoreApplication::translate("GroupVisibilityWindow", "\320\222\320\270\320\264\320\270\320\274\320\276\321\201\321\202\321\214 \320\262\320\275\321\203\321\202\321\200\320\270 \320\263\321\200\321\203\320\277\320\277\320\270\321\200\320\276\320\262\320\272\320\270", nullptr));
        btnNext->setText(QCoreApplication::translate("GroupVisibilityWindow", "\320\224\320\260\320\273\320\265\320\265", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GroupVisibilityWindow: public Ui_GroupVisibilityWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GROUPVISIBILITYWINDOW_H
