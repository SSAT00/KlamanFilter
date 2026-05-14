#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include "UtochnenieSimple/utochnenie.h"
#include <QSplitter>
#include <QBoxLayout>
#include <QToolBox>
#include <QScrollArea>
#include "chartdialog.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Utochnenie* appFilter = new Utochnenie();

    void RunKalmanFilter();

    void SetDataStart(QDateTime);
    void SetSatGroups(QVector<QString>, QVector<int>, Matrix<QString, Dynamic, 1>, QVector<int>);

    QDateTime GetPataFinish();
    void GetDtParams(long double&, long double&, long double&, long double&);
    int GetNParams();
    void GetPerts(int&, int&, bool&, bool&, bool&, bool&, bool&, bool&);
    void GetNoises(long double&, long double&, long double &);
    void GetDempf(long double&);
    void GetAcc(long double&);

    void SetNIters(int, int);
    void ClearNIters();
    void SetMean(double, int);
    void ClearMean();
    void SetSTD(double, int);
    void ClearSTD();
    void SetCond(double, int);
    void ClearCond();
    void SetDqX(double, int);
    void ClearDqX();
    void SetDqSD(double, int);
    void ClearDqSD();
    void SetNObs(int, int);
    void ClearNObs();

    void SetLocalProgress(int, QString, QString);
    void SetProgress(int, QString);

    void CloseStartFrames();

private:
    Ui::MainWindow *ui;

    bool fl_stop;
    QVector<QString> Groups, Names;
    QVector<int> SatsNGroup, GroupsN;
    QVector<QLabel*> labels_iter, labels_mean, labels_std, labels_cond, labels_dqx, labels_dqsd, labels_nobs;
    QVector<QPushButton*> buttons_chart;

    void ButtonChartClick(int);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_btnRunProcces_clicked();

signals:
    void runKalmanFilter();
    void windowClosed();

};
#endif // MAINWINDOW_H
