#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chartdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->cbSysErr, &QCheckBox::stateChanged, this, [=](int state) {
        if (state == Qt::Checked) {
            ui->leSysErr->setEnabled(true);
        } else {
            ui->leSysErr->setEnabled(false);
        }
    });

    fl_stop = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    emit windowClosed();
    event->accept();
}

void MainWindow::on_btnRunProcces_clicked(){
    if (!fl_stop){
        emit runKalmanFilter();
        fl_stop = true;
        ui->btnRunProcces->setText("Завершить");
    }else{
        appFilter->stop();
        ui->btnRunProcces->setEnabled(false);
    }
}

void MainWindow::SetDataStart(QDateTime dt){
    ui->dteStart->setDateTime(dt);
    ui->dtePars->setDateTime(dt);
}

void MainWindow::SetSatGroups(QVector<QString> g, QVector<int> n, Matrix<QString, Dynamic, 1> names, QVector<int>n_groups){
    Groups = g;
    SatsNGroup = n;
    GroupsN = n_groups;
    for(int i = 0 ; i < names.size(); i++){
        Names.append(names(i));
    }

    QFrame* fSatCalcInfo;

    fSatCalcInfo = new QFrame();
    fSatCalcInfo->setObjectName("fInfoCalc");
    fSatCalcInfo->setMinimumSize(100, 100);

    QGridLayout * glForTlbInfo = new QGridLayout(fSatCalcInfo);

    QFont Font("Montserrat", 9);
    Font.setWeight(900);

    QToolBox * tlbCalcInfo = new QToolBox();

    int lw = 100;

    for(int i = 0; i < Groups.size(); i++){
        Font.setPointSize(8);
        QWidget* newPage = new QWidget();
        QGridLayout* prelayout = new QGridLayout(newPage);

        newPage->setFont(Font);

        QScrollArea* preframe = new QScrollArea();

        QWidget *container = new QWidget();

        QGridLayout* layout = new QGridLayout(container);

        layout->setVerticalSpacing(20);
        layout->setHorizontalSpacing(50);

        QLabel *l_name_sat = new QLabel();
        l_name_sat->setText("Название");
        l_name_sat->setMinimumSize(lw, 40);
        Font.setWeight(900);
        l_name_sat->setAlignment(Qt::AlignCenter);
        l_name_sat->setFont(Font);
        layout->addWidget(l_name_sat, 0, 0);

        QLabel *l_mean_sat = new QLabel();
        l_mean_sat->setText("Среднее невязок, см");
        l_mean_sat->setWordWrap(true);
        l_mean_sat->setMinimumSize(lw, 40);
        l_mean_sat->setFont(Font);
        l_mean_sat->setAlignment(Qt::AlignCenter);
        layout->addWidget(l_mean_sat, 0, 1);

        QLabel *l_std_sat = new QLabel();
        l_std_sat->setText("Отклонения, см");
        l_std_sat->setMinimumSize(lw, 40);
        l_std_sat->setFont(Font);
        l_std_sat->setAlignment(Qt::AlignCenter);
        layout->addWidget(l_std_sat, 0, 2);

        QLabel *l_niter_sat = new QLabel();
        l_niter_sat->setText("Итерации");
        l_niter_sat->setMinimumSize(lw, 40);
        l_niter_sat->setFont(Font);
        l_niter_sat->setAlignment(Qt::AlignCenter);
        layout->addWidget(l_niter_sat, 0, 3);

        QLabel *l_cond_sat = new QLabel();
        l_cond_sat->setText("Обусловленость");
        l_cond_sat->setMinimumSize(lw, 40);
        l_cond_sat->setFont(Font);
        l_cond_sat->setAlignment(Qt::AlignCenter);
        layout->addWidget(l_cond_sat, 0, 4);

        QLabel *l_dqx_sat = new QLabel();
        l_dqx_sat->setText("Кинематические поправки");
        l_dqx_sat->setWordWrap(true);
        l_dqx_sat->setMinimumSize(lw, 40);
        l_dqx_sat->setFont(Font);
        l_dqx_sat->setAlignment(Qt::AlignCenter);
        layout->addWidget(l_dqx_sat, 0, 5);

        QLabel *l_dqsd_sat = new QLabel();
        l_dqsd_sat->setText("Поправки параметров СД");
        l_dqsd_sat->setWordWrap(true);
        l_dqsd_sat->setMinimumSize(lw, 40);
        l_dqsd_sat->setFont(Font);
        l_dqsd_sat->setAlignment(Qt::AlignCenter);
        layout->addWidget(l_dqsd_sat, 0, 6);

        QLabel *l_nobs_sat = new QLabel();
        l_nobs_sat->setText("Число наблюдений");
        l_nobs_sat->setWordWrap(true);
        l_nobs_sat->setMinimumSize(lw, 40);
        l_nobs_sat->setFont(Font);
        l_nobs_sat->setAlignment(Qt::AlignCenter);
        layout->addWidget(l_nobs_sat, 0, 7);

        int k = 0;

        for(int j = 0; j < SatsNGroup.size(); j++){
            Font.setPointSize(10);
            QLabel* l = new QLabel();
            l->setText(Names[j]);
            l->setAlignment(Qt::AlignCenter);
            l->setMinimumSize(lw, 40);
            l->setMaximumSize(lw, 40);
            Font.setWeight(700);
            l->setFont(Font);
            if (SatsNGroup[j] == GroupsN[i]){
                Font.setWeight(900);

                QLabel* l_iters = new QLabel();
                l_iters->setText("-");
                l_iters->setFont(Font);
                l_iters->setMinimumSize(lw, 40);
                l_iters->setAlignment(Qt::AlignCenter);
                labels_iter.append(l_iters);

                QLabel* l_mean = new QLabel();
                l_mean->setText("-");
                l_mean->setFont(Font);
                l_mean->setMinimumSize(lw, 40);
                l_mean->setAlignment(Qt::AlignCenter);
                labels_mean.append(l_mean);

                QLabel* l_std = new QLabel();
                l_std->setText("-");
                l_std->setFont(Font);
                l_std->setMinimumSize(lw, 40);
                l_std->setAlignment(Qt::AlignCenter);
                labels_std.append(l_std);

                QLabel* l_cond = new QLabel();
                l_cond->setText("-");
                l_cond->setFont(Font);
                l_cond->setMinimumSize(lw, 40);
                l_cond->setAlignment(Qt::AlignCenter);
                labels_cond.append(l_cond);

                QLabel* l_dqx = new QLabel();
                l_dqx->setText("-");
                l_dqx->setFont(Font);
                l_dqx->setMinimumSize(lw, 40);
                l_dqx->setAlignment(Qt::AlignCenter);
                labels_dqx.append(l_dqx);

                QLabel* l_dqsd = new QLabel();
                l_dqsd->setText("-");
                l_dqsd->setFont(Font);
                l_dqsd->setMinimumSize(lw, 40);
                l_dqsd->setAlignment(Qt::AlignCenter);
                labels_dqsd.append(l_dqsd);

                QLabel* l_nobs = new QLabel();
                l_nobs->setText("-");
                l_nobs->setFont(Font);
                l_nobs->setMinimumSize(lw, 40);
                l_nobs->setAlignment(Qt::AlignCenter);
                labels_nobs.append(l_nobs);

                QPushButton* btn_chart = new QPushButton();
                btn_chart->setText("График");
                Font.setPointSize(8);
                btn_chart->setFont(Font);
                Font.setPointSize(10);
                btn_chart->setMinimumSize(lw+40, 40);
                buttons_chart.append(btn_chart);

                layout->addWidget(l_iters, k+1, 3);
                layout->addWidget(l_mean, k+1, 1);
                layout->addWidget(l_std, k+1, 2);
                layout->addWidget(l_cond, k+1, 4);
                layout->addWidget(l_dqx, k+1, 5);
                layout->addWidget(l_dqsd, k+1, 6);
                layout->addWidget(l_nobs, k+1, 7);
                layout->addWidget(btn_chart, k+1, 8);
                layout->addWidget(l, k+1, 0);

                k++;
            }
        }
        preframe->setWidget(container);

        prelayout->addWidget(preframe, 0, 0, 1, 3);

        QString sectionName = Groups[i];

        Font.setWeight(900);
        newPage->setFont(Font);
        tlbCalcInfo->addItem(newPage, sectionName);
    }

    for (int i = 0; i < buttons_chart.size(); ++i) {
        connect(buttons_chart[i], &QPushButton::clicked, this, [this, i]() {
            ButtonChartClick(i);
        });
    }

    tlbCalcInfo->setFont(Font);

    glForTlbInfo->addWidget(tlbCalcInfo, 0, 0);

    fSatCalcInfo->setStyleSheet("QFrame#fInfoCalc{border:1px solid #D3D3D3;border-radius:20px; background-color:#fff;}");

    ui->gridLayout_5->addWidget(fSatCalcInfo, 0, 0, 1, 3);
}

void MainWindow::ButtonChartClick(int i){

    ChartDialog dlg(appFilter->GetSatName(i), this);
    dlg.exec();
}


QDateTime MainWindow::GetPataFinish(){
    QDateTime dt = ui->dteFinish->dateTime();
    return dt;
}

int MainWindow::GetNParams(){
    if (ui->rb6->isChecked()){
        return 6;
    }else{
        return 15;
    }
}

void MainWindow::GetPerts(int & nm, int & mn, bool & sun, bool & moon, bool & plan, bool & lp, bool & rel, bool & tides){
    nm = ui->sbNM->text().toInt();
    mn = ui->sbMN->text().toInt();
    sun = ui->cbSun->isChecked();
    moon = ui->cbMoon->isChecked();
    plan = ui->cbPlans->isChecked();
    lp = ui->cbLP->isChecked();
    rel = ui->cbRel->isChecked();
    tides = ui->cbTides->isChecked();
}

void MainWindow::GetDtParams(long double& dt, long double& block, long double& gap, long double& interval){
    interval = stold(ui->dteInterval->text().toStdString());
    gap = stold(ui->dteGap->text().toStdString());
    block = stold(ui->dteBlock->text().toStdString());
    dt = stold(ui->dteDt->text().toStdString());
}

void MainWindow::GetNoises(long double & sat, long double & stan, long double & sys){
    sat = stold(ui->leSatErr->text().toStdString());
    stan = stold(ui->leStanErr->text().toStdString());
    if (ui->cbSysErr->isChecked()){
        sys = stold(ui->leSysErr->text().toStdString());
    }else{
        sys = 0.0;
    }
}

void MainWindow::GetDempf(long double & dempf){
    dempf = stold(ui->leDempf->text().toStdString());
}

void MainWindow::GetAcc(long double & acc){
    acc = stold(ui->leAcc->text().toStdString());
}

void MainWindow::CloseStartFrames(){
    ui->fNavFrames->hide();
    ui->fPerturb->hide();
    ui->fOther->hide();
}

void MainWindow::SetNIters(int n, int n_iter){
    labels_iter[n]->setText(QString::number(n_iter));
    labels_iter[n]->setStyleSheet("color:#449931;");
}

void MainWindow::ClearNIters(){
    for(int i = 0; i < Names.size(); i++){
        labels_iter[i]->setStyleSheet("color:#646464;");
        labels_iter[i]->setText("ожидание");
    }
}

void MainWindow::SetMean(double mean, int n){
    labels_mean[n]->setText(QString::number(mean));
    labels_mean[n]->setStyleSheet("color:#449931;");
}

void MainWindow::ClearMean(){
    for(int i = 0; i < Names.size(); i++){
        labels_mean[i]->setStyleSheet("color:#646464;");
        labels_mean[i]->setText("ожидание");
    }
}

void MainWindow::SetSTD(double std, int n){
    labels_std[n]->setText(QString::number(std));
    labels_std[n]->setStyleSheet("color:#449931;");
}

void MainWindow::ClearSTD(){
    for(int i = 0; i < Names.size(); i++){
        labels_std[i]->setStyleSheet("color:#646464;");
        labels_std[i]->setText("ожидание");
    }
}

void MainWindow::SetCond(double std, int n){
    labels_cond[n]->setText(QString::number(std));
    labels_cond[n]->setStyleSheet("color:#449931;");
}

void MainWindow::ClearCond(){
    for(int i = 0; i < Names.size(); i++){
        labels_cond[i]->setStyleSheet("color:#646464;");
        labels_cond[i]->setText("ожидание");
    }
}

void MainWindow::SetDqX(double dqx, int n){
    labels_dqx[n]->setText(QString::number(dqx));
    labels_dqx[n]->setStyleSheet("color:#449931;");
}

void MainWindow::ClearDqX(){
    for(int i = 0; i < Names.size(); i++){
        labels_dqx[i]->setStyleSheet("color:#646464;");
        labels_dqx[i]->setText("ожидание");
    }
}

void MainWindow::SetDqSD(double dqsd, int n){
    labels_dqsd[n]->setText(QString::number(dqsd));
    labels_dqsd[n]->setStyleSheet("color:#449931;");
}

void MainWindow::ClearDqSD(){
    for(int i = 0; i < Names.size(); i++){
        labels_dqsd[i]->setStyleSheet("color:#646464;");
        labels_dqsd[i]->setText("ожидание");
    }
}

void MainWindow::SetNObs(int nobs, int n){
    labels_nobs[n]->setText(QString::number(nobs));
    labels_nobs[n]->setStyleSheet("color:#449931;");
}

void MainWindow::ClearNObs(){
    for(int i = 0; i < Names.size(); i++){
        labels_nobs[i]->setStyleSheet("color:#646464;");
        labels_nobs[i]->setText("ожидание");
    }
}

void MainWindow::SetLocalProgress(int percents, QString stage, QString name){
    ui->pbLocalProgress->setValue(percents);
    ui->lLocalProgress->setText(stage + " " + name);
}

void MainWindow::SetProgress(int percents, QString text){
    ui->pbProgress->setValue(percents);
    ui->lProgress->setText(text);
}











