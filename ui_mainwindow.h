/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QDate>
#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QFrame *frame;
    QGridLayout *gridLayout_6;
    QProgressBar *pbProgress;
    QLabel *lProgress;
    QProgressBar *pbLocalProgress;
    QLabel *lLocalProgress;
    QFrame *fHeader;
    QGridLayout *gridLayout_5;
    QFrame *fOther;
    QGridLayout *gridLayout_4;
    QLineEdit *leDempf;
    QLabel *label_11;
    QLineEdit *lineEdit;
    QRadioButton *rb6;
    QCheckBox *cbSysErr;
    QRadioButton *rb15;
    QLineEdit *leSatErr;
    QLabel *label_13;
    QLabel *label_10;
    QLineEdit *leAcc;
    QLabel *label_12;
    QLineEdit *leStanErr;
    QLabel *label_14;
    QLabel *label_15;
    QLineEdit *leSysErr;
    QSpacerItem *verticalSpacer_2;
    QFrame *fNavFrames;
    QGridLayout *gridLayout_2;
    QDateTimeEdit *dteFinish;
    QLineEdit *dteInterval;
    QLabel *label_7;
    QLabel *label_3;
    QLabel *label_2;
    QLineEdit *dteGap;
    QDateTimeEdit *dteStart;
    QLabel *label_5;
    QLineEdit *dteDt;
    QDateTimeEdit *dtePars;
    QLabel *label;
    QLineEdit *dteBlock;
    QLabel *label_6;
    QLabel *label_4;
    QSpacerItem *verticalSpacer_3;
    QPushButton *btnRunProcces;
    QFrame *fPerturb;
    QGridLayout *gridLayout_3;
    QCheckBox *cbTides;
    QSpinBox *sbMN;
    QCheckBox *cbMoon;
    QCheckBox *cbSun;
    QSpinBox *sbNM;
    QLabel *label_8;
    QCheckBox *cbRel;
    QCheckBox *cbPlans;
    QLabel *label_9;
    QCheckBox *cbLP;
    QSpacerItem *verticalSpacer;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1420, 1024);
        MainWindow->setMinimumSize(QSize(1200, 600));
        QFont font;
        font.setFamily(QString::fromUtf8("Montserrat"));
        MainWindow->setFont(font);
        QIcon icon;
        icon.addFile(QString::fromUtf8("img/IconSatellite.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setSpacing(10);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(20, 20, 20, 0);
        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setMinimumSize(QSize(0, 100));
        frame->setMaximumSize(QSize(16777215, 100));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_6 = new QGridLayout(frame);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        pbProgress = new QProgressBar(frame);
        pbProgress->setObjectName(QString::fromUtf8("pbProgress"));
        pbProgress->setValue(0);

        gridLayout_6->addWidget(pbProgress, 4, 0, 1, 1);

        lProgress = new QLabel(frame);
        lProgress->setObjectName(QString::fromUtf8("lProgress"));
        lProgress->setMinimumSize(QSize(400, 0));
        lProgress->setMaximumSize(QSize(400, 16777215));

        gridLayout_6->addWidget(lProgress, 4, 1, 1, 1);

        pbLocalProgress = new QProgressBar(frame);
        pbLocalProgress->setObjectName(QString::fromUtf8("pbLocalProgress"));
        pbLocalProgress->setValue(0);

        gridLayout_6->addWidget(pbLocalProgress, 5, 0, 1, 1);

        lLocalProgress = new QLabel(frame);
        lLocalProgress->setObjectName(QString::fromUtf8("lLocalProgress"));
        lLocalProgress->setMaximumSize(QSize(400, 400));

        gridLayout_6->addWidget(lLocalProgress, 5, 1, 1, 2);


        gridLayout->addWidget(frame, 6, 0, 1, 2);

        fHeader = new QFrame(centralwidget);
        fHeader->setObjectName(QString::fromUtf8("fHeader"));
        fHeader->setLayoutDirection(Qt::LeftToRight);
        fHeader->setFrameShape(QFrame::StyledPanel);
        fHeader->setFrameShadow(QFrame::Raised);
        gridLayout_5 = new QGridLayout(fHeader);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        fOther = new QFrame(fHeader);
        fOther->setObjectName(QString::fromUtf8("fOther"));
        fOther->setMinimumSize(QSize(500, 0));
        fOther->setStyleSheet(QString::fromUtf8("QFrame#fOther{\n"
"	border:1px solid #D3D3D3;\n"
"	border-radius:20px;\n"
"}"));
        fOther->setFrameShape(QFrame::StyledPanel);
        fOther->setFrameShadow(QFrame::Raised);
        gridLayout_4 = new QGridLayout(fOther);
        gridLayout_4->setSpacing(10);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setContentsMargins(20, 20, 20, 20);
        leDempf = new QLineEdit(fOther);
        leDempf->setObjectName(QString::fromUtf8("leDempf"));
        leDempf->setMaximumSize(QSize(180, 16777215));
        leDempf->setFont(font);

        gridLayout_4->addWidget(leDempf, 4, 0, 1, 1);

        label_11 = new QLabel(fOther);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setFont(font);

        gridLayout_4->addWidget(label_11, 1, 1, 1, 2);

        lineEdit = new QLineEdit(fOther);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setMaximumSize(QSize(180, 16777215));
        lineEdit->setFont(font);

        gridLayout_4->addWidget(lineEdit, 7, 0, 1, 1);

        rb6 = new QRadioButton(fOther);
        rb6->setObjectName(QString::fromUtf8("rb6"));
        rb6->setMaximumSize(QSize(40, 16777215));
        rb6->setFont(font);

        gridLayout_4->addWidget(rb6, 0, 1, 1, 1);

        cbSysErr = new QCheckBox(fOther);
        cbSysErr->setObjectName(QString::fromUtf8("cbSysErr"));
        cbSysErr->setEnabled(true);
        cbSysErr->setFont(font);
        cbSysErr->setChecked(true);

        gridLayout_4->addWidget(cbSysErr, 3, 1, 1, 2);

        rb15 = new QRadioButton(fOther);
        rb15->setObjectName(QString::fromUtf8("rb15"));
        rb15->setFont(font);
        rb15->setChecked(true);

        gridLayout_4->addWidget(rb15, 0, 2, 1, 1);

        leSatErr = new QLineEdit(fOther);
        leSatErr->setObjectName(QString::fromUtf8("leSatErr"));
        leSatErr->setMaximumSize(QSize(180, 16777215));
        leSatErr->setFont(font);

        gridLayout_4->addWidget(leSatErr, 1, 0, 1, 1);

        label_13 = new QLabel(fOther);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setFont(font);

        gridLayout_4->addWidget(label_13, 4, 1, 1, 2);

        label_10 = new QLabel(fOther);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setMaximumSize(QSize(160, 16777215));
        label_10->setFont(font);

        gridLayout_4->addWidget(label_10, 0, 0, 1, 1);

        leAcc = new QLineEdit(fOther);
        leAcc->setObjectName(QString::fromUtf8("leAcc"));
        leAcc->setMaximumSize(QSize(180, 16777215));
        leAcc->setFont(font);

        gridLayout_4->addWidget(leAcc, 6, 0, 1, 1);

        label_12 = new QLabel(fOther);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setFont(font);

        gridLayout_4->addWidget(label_12, 2, 1, 1, 2);

        leStanErr = new QLineEdit(fOther);
        leStanErr->setObjectName(QString::fromUtf8("leStanErr"));
        leStanErr->setMaximumSize(QSize(180, 16777215));
        leStanErr->setFont(font);

        gridLayout_4->addWidget(leStanErr, 2, 0, 1, 1);

        label_14 = new QLabel(fOther);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setFont(font);

        gridLayout_4->addWidget(label_14, 6, 1, 1, 2);

        label_15 = new QLabel(fOther);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setFont(font);

        gridLayout_4->addWidget(label_15, 7, 1, 1, 2);

        leSysErr = new QLineEdit(fOther);
        leSysErr->setObjectName(QString::fromUtf8("leSysErr"));
        leSysErr->setEnabled(true);
        leSysErr->setMaximumSize(QSize(180, 16777215));
        leSysErr->setFont(font);

        gridLayout_4->addWidget(leSysErr, 3, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer_2, 8, 0, 1, 3);


        gridLayout_5->addWidget(fOther, 0, 2, 1, 1);

        fNavFrames = new QFrame(fHeader);
        fNavFrames->setObjectName(QString::fromUtf8("fNavFrames"));
        fNavFrames->setMinimumSize(QSize(500, 0));
        fNavFrames->setFont(font);
        fNavFrames->setStyleSheet(QString::fromUtf8("QFrame#fNavFrames{\n"
"	border:1px solid #D3D3D3;\n"
"	border-radius:20px;\n"
"}"));
        fNavFrames->setFrameShape(QFrame::StyledPanel);
        fNavFrames->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(fNavFrames);
        gridLayout_2->setSpacing(10);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(20, 20, 20, 20);
        dteFinish = new QDateTimeEdit(fNavFrames);
        dteFinish->setObjectName(QString::fromUtf8("dteFinish"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Montserrat"));
        font1.setPointSize(8);
        dteFinish->setFont(font1);
        dteFinish->setDate(QDate(2021, 3, 1));

        gridLayout_2->addWidget(dteFinish, 2, 0, 1, 1);

        dteInterval = new QLineEdit(fNavFrames);
        dteInterval->setObjectName(QString::fromUtf8("dteInterval"));
        dteInterval->setMaximumSize(QSize(200, 16777215));
        dteInterval->setFont(font);

        gridLayout_2->addWidget(dteInterval, 7, 0, 1, 1);

        label_7 = new QLabel(fNavFrames);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setFont(font);

        gridLayout_2->addWidget(label_7, 7, 1, 1, 2);

        label_3 = new QLabel(fNavFrames);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font);

        gridLayout_2->addWidget(label_3, 2, 1, 1, 2);

        label_2 = new QLabel(fNavFrames);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setEnabled(false);
        label_2->setFont(font);

        gridLayout_2->addWidget(label_2, 0, 1, 1, 2);

        dteGap = new QLineEdit(fNavFrames);
        dteGap->setObjectName(QString::fromUtf8("dteGap"));
        dteGap->setMaximumSize(QSize(200, 16777215));
        dteGap->setFont(font);

        gridLayout_2->addWidget(dteGap, 6, 0, 1, 1);

        dteStart = new QDateTimeEdit(fNavFrames);
        dteStart->setObjectName(QString::fromUtf8("dteStart"));
        dteStart->setEnabled(false);
        dteStart->setFont(font);
        dteStart->setDate(QDate(2026, 1, 1));

        gridLayout_2->addWidget(dteStart, 1, 0, 1, 1);

        label_5 = new QLabel(fNavFrames);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setFont(font);

        gridLayout_2->addWidget(label_5, 5, 1, 1, 2);

        dteDt = new QLineEdit(fNavFrames);
        dteDt->setObjectName(QString::fromUtf8("dteDt"));
        dteDt->setMaximumSize(QSize(200, 16777215));
        dteDt->setFont(font);

        gridLayout_2->addWidget(dteDt, 4, 0, 1, 1);

        dtePars = new QDateTimeEdit(fNavFrames);
        dtePars->setObjectName(QString::fromUtf8("dtePars"));
        dtePars->setEnabled(false);
        dtePars->setFont(font1);
        dtePars->setDate(QDate(2026, 1, 1));

        gridLayout_2->addWidget(dtePars, 0, 0, 1, 1);

        label = new QLabel(fNavFrames);
        label->setObjectName(QString::fromUtf8("label"));
        label->setEnabled(false);
        label->setFont(font);

        gridLayout_2->addWidget(label, 1, 1, 1, 2);

        dteBlock = new QLineEdit(fNavFrames);
        dteBlock->setObjectName(QString::fromUtf8("dteBlock"));
        dteBlock->setMaximumSize(QSize(200, 16777215));
        dteBlock->setFont(font);

        gridLayout_2->addWidget(dteBlock, 5, 0, 1, 1);

        label_6 = new QLabel(fNavFrames);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setFont(font);

        gridLayout_2->addWidget(label_6, 6, 1, 1, 2);

        label_4 = new QLabel(fNavFrames);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setFont(font);

        gridLayout_2->addWidget(label_4, 4, 1, 1, 2);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_3, 8, 0, 1, 3);


        gridLayout_5->addWidget(fNavFrames, 0, 0, 1, 1);

        btnRunProcces = new QPushButton(fHeader);
        btnRunProcces->setObjectName(QString::fromUtf8("btnRunProcces"));
        btnRunProcces->setMinimumSize(QSize(180, 40));
        btnRunProcces->setStyleSheet(QString::fromUtf8(""));

        gridLayout_5->addWidget(btnRunProcces, 1, 1, 1, 1);

        fPerturb = new QFrame(fHeader);
        fPerturb->setObjectName(QString::fromUtf8("fPerturb"));
        fPerturb->setMinimumSize(QSize(200, 0));
        fPerturb->setMaximumSize(QSize(200, 16777215));
        fPerturb->setStyleSheet(QString::fromUtf8("QFrame#fPerturb{\n"
"	border:1px solid #D3D3D3;\n"
"	border-radius:20px;\n"
"}"));
        fPerturb->setFrameShape(QFrame::StyledPanel);
        fPerturb->setFrameShadow(QFrame::Raised);
        gridLayout_3 = new QGridLayout(fPerturb);
        gridLayout_3->setSpacing(10);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(20, 20, 20, 20);
        cbTides = new QCheckBox(fPerturb);
        cbTides->setObjectName(QString::fromUtf8("cbTides"));
        cbTides->setFont(font);

        gridLayout_3->addWidget(cbTides, 8, 0, 1, 3);

        sbMN = new QSpinBox(fPerturb);
        sbMN->setObjectName(QString::fromUtf8("sbMN"));
        sbMN->setFont(font);
        sbMN->setMaximum(360);
        sbMN->setValue(12);

        gridLayout_3->addWidget(sbMN, 1, 0, 1, 2);

        cbMoon = new QCheckBox(fPerturb);
        cbMoon->setObjectName(QString::fromUtf8("cbMoon"));
        cbMoon->setFont(font);
        cbMoon->setChecked(true);

        gridLayout_3->addWidget(cbMoon, 4, 0, 1, 1);

        cbSun = new QCheckBox(fPerturb);
        cbSun->setObjectName(QString::fromUtf8("cbSun"));
        cbSun->setFont(font);
        cbSun->setChecked(true);

        gridLayout_3->addWidget(cbSun, 2, 0, 1, 3);

        sbNM = new QSpinBox(fPerturb);
        sbNM->setObjectName(QString::fromUtf8("sbNM"));
        sbNM->setFont(font);
        sbNM->setMinimum(0);
        sbNM->setMaximum(360);
        sbNM->setSingleStep(1);
        sbNM->setValue(12);

        gridLayout_3->addWidget(sbNM, 0, 0, 1, 2);

        label_8 = new QLabel(fPerturb);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setFont(font);

        gridLayout_3->addWidget(label_8, 0, 2, 1, 1);

        cbRel = new QCheckBox(fPerturb);
        cbRel->setObjectName(QString::fromUtf8("cbRel"));
        cbRel->setFont(font);

        gridLayout_3->addWidget(cbRel, 7, 0, 1, 3);

        cbPlans = new QCheckBox(fPerturb);
        cbPlans->setObjectName(QString::fromUtf8("cbPlans"));
        cbPlans->setFont(font);
        cbPlans->setChecked(false);

        gridLayout_3->addWidget(cbPlans, 5, 0, 1, 3);

        label_9 = new QLabel(fPerturb);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setFont(font);

        gridLayout_3->addWidget(label_9, 1, 2, 1, 1);

        cbLP = new QCheckBox(fPerturb);
        cbLP->setObjectName(QString::fromUtf8("cbLP"));
        cbLP->setFont(font);
        cbLP->setChecked(true);

        gridLayout_3->addWidget(cbLP, 6, 0, 1, 3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer, 9, 0, 1, 3);


        gridLayout_5->addWidget(fPerturb, 0, 1, 1, 1);


        gridLayout->addWidget(fHeader, 4, 0, 1, 2);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1420, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\320\224\320\270\320\275\320\260\320\274\320\270\321\207\320\265\321\201\320\272\320\260\321\217 \321\200\320\265\320\272\321\203\321\200\321\201\320\270\320\262\320\275\320\260\321\217 \321\204\320\270\320\273\321\214\321\202\321\200\320\260\321\206\320\270\321\217", nullptr));
        lProgress->setText(QString());
        lLocalProgress->setText(QString());
        leDempf->setText(QCoreApplication::translate("MainWindow", "0.75", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "\320\241\321\202\320\276\321\205\320\260\321\201\321\202\320\270\321\207\320\265\321\201\320\272\320\270\320\265 \321\210\321\203\320\274\321\213 \321\201\320\277\321\203\321\202\320\275\320\270\320\272\320\276\320\262, \321\201\320\274", nullptr));
        lineEdit->setText(QCoreApplication::translate("MainWindow", "2", nullptr));
        rb6->setText(QCoreApplication::translate("MainWindow", "6", nullptr));
        cbSysErr->setText(QCoreApplication::translate("MainWindow", "\320\241\320\270\321\201\321\202\320\265\320\274\320\260\321\202\320\270\321\207\320\265\321\201\320\272\320\270\320\265 \321\210\321\203\320\274\321\213", nullptr));
        rb15->setText(QCoreApplication::translate("MainWindow", "15", nullptr));
        leSatErr->setText(QCoreApplication::translate("MainWindow", "30", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "\320\224\321\215\320\274\320\277\321\204\320\270\321\200\321\203\321\216\321\211\320\270\320\271 \320\274\320\275\320\276\320\266\320\270\321\202\320\265\320\273\321\214", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "\320\243\321\202\320\276\321\207\320\275\321\217\321\202\321\214 \320\277\320\260\321\200\320\260\320\274\320\265\321\202\321\200\321\213", nullptr));
        leAcc->setText(QCoreApplication::translate("MainWindow", "1e-5", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "\320\241\321\202\320\276\321\205\320\260\321\201\321\202\320\270\321\207\320\265\321\201\320\272\320\270\320\265 \321\210\321\203\320\274\321\213 \321\201\321\202\320\260\320\275\321\206\320\270\320\271, \321\201\320\274", nullptr));
        leStanErr->setText(QCoreApplication::translate("MainWindow", "30", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "\320\232\321\200\320\270\321\202\320\265\321\200\320\270\320\271 \320\262\321\213\321\205\320\276\320\264\320\260", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "\320\234\320\260\320\272\321\201\320\270\320\274\320\260\320\273\321\214\320\275\320\276\320\265 \321\207\320\270\321\201\320\273\320\276 \320\270\321\202\320\265\321\200\320\260\321\206\320\270\320\271", nullptr));
        leSysErr->setText(QCoreApplication::translate("MainWindow", "30", nullptr));
        dteFinish->setDisplayFormat(QCoreApplication::translate("MainWindow", "yyyy.MM.dd hh.mm.ss", nullptr));
        dteInterval->setText(QCoreApplication::translate("MainWindow", "172800.0", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "\320\234\320\265\321\200\320\275\321\213\320\271 \320\270\320\275\321\202\320\265\321\200\320\262\320\260\320\273, \321\201", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\320\232\320\276\320\275\320\265\321\206 \320\275\320\260\320\262\320\270\320\263\320\260\321\206\320\270\320\270", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\320\255\320\277\320\276\321\205\320\260 \320\277\320\260\321\200\320\260\320\274\320\265\321\202\321\200\320\276\320\262", nullptr));
        dteGap->setText(QCoreApplication::translate("MainWindow", "0.0", nullptr));
        dteStart->setDisplayFormat(QCoreApplication::translate("MainWindow", "yyyy.MM.dd hh.mm.ss", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "\320\221\320\273\320\276\320\272 \320\275\320\260\320\261\320\273\321\216\320\264\320\265\320\275\320\270\320\271, \321\201", nullptr));
        dteDt->setText(QCoreApplication::translate("MainWindow", "20.0", nullptr));
        dtePars->setDisplayFormat(QCoreApplication::translate("MainWindow", "yyyy.MM.dd hh.mm.ss", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\320\235\320\260\321\207\320\260\320\273\320\276 \320\275\320\260\320\262\320\270\320\263\320\260\321\206\320\270\320\270", nullptr));
        dteBlock->setText(QCoreApplication::translate("MainWindow", "3600.0", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "\320\240\320\260\320\267\321\200\321\213\320\262 \320\274\320\265\320\266\320\264\321\203 \320\261\320\273\320\276\320\272\320\260\320\274\320\270 \320\275\320\260\320\261\320\273\321\216\320\264\320\265\320\275\320\270\320\271, \321\201", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\320\222\321\200\320\265\320\274\321\217 \320\274\320\265\320\266\320\264\321\203 \320\275\320\260\320\261\320\273\321\216\320\264\320\265\320\275\320\270\321\217\320\274\320\270, \321\201", nullptr));
        btnRunProcces->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\320\277\321\203\321\201\321\202\320\270\321\202\321\214", nullptr));
        cbTides->setText(QCoreApplication::translate("MainWindow", "\320\242\320\262\320\265\321\200\320\264\321\213\320\265 \320\277\321\200\320\270\320\273\320\270\320\262\321\213", nullptr));
        cbMoon->setText(QCoreApplication::translate("MainWindow", "\320\233\321\203\320\275\320\260", nullptr));
        cbSun->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\320\273\320\275\321\206\320\265", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "NM", nullptr));
        cbRel->setText(QCoreApplication::translate("MainWindow", "\320\240\320\265\320\273\321\217\321\202\320\270\320\262\320\270\321\201\321\202\320\270\320\272\320\260", nullptr));
        cbPlans->setText(QCoreApplication::translate("MainWindow", "\320\237\320\273\320\260\320\275\320\265\321\202\321\213", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "MN", nullptr));
        cbLP->setText(QCoreApplication::translate("MainWindow", "\320\241\320\262\320\265\321\202\320\276\320\262\320\276\320\265 \320\264\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
