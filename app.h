#ifndef APP_H
#define APP_H

#include "mainwindow.h"
#include "welcomewindow.h"
#include "groupvisibilitywindow.h"
#include "choosesatelliteswindow.h"
#include "UtochnenieSimple/Modules.h"

#include <QtConcurrent>

class App : public QObject {
    Q_OBJECT

public:
    explicit App(QObject *parent = nullptr);
    void start();
    void PrepareDataForStart();

public slots:
    void showOpenChooseSatellitesWindow();
    void showOpenChooseVisibilityGroupWindow();
    void showOpenMainWindow();

private:
    WelcomeWindow* welcomewin;
    ChooseSatellitesWindow* choosesatwin;
    GroupVisibilityWindow* groupvisibilitywin;
    MainWindow* mainwin;

    QVector<ChoosedSatellite> Satellites;
    QVector<ChoosedGroup> Groups;
    QVector<ChoosedStantion> Stantions;
    QVector<QString> GroupsNames;
    QVector<int> SatsNGroup, GroupsNum;

    int nsat, nstan;

    Matrix<long double, Dynamic, Dynamic> SatCoords, StanCoords;

    Matrix<int, Dynamic, Dynamic> cycl_sat, cycl_stan;

    Matrix<long double, Dynamic, 9> lpc;

    Matrix<long double, Dynamic, 2> angles_sat, angles_stan;

    Matrix<QString, Dynamic, 1> SatNames;

    void Run();

    void SetData2App();
};

#endif // APP_H
