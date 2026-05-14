#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H


#include "UtochnenieSimple/Modules.h"
#include "DBWorker/dbworker.h"


namespace Ui {
class WelcomeWindow;
}

class WelcomeWindow : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomeWindow(QWidget *parent = nullptr);
    ~WelcomeWindow();

    bool UseStan();

    QVector<ChoosedSatellite> GetSatellites();
    QVector<ChoosedGroup> GetGroups();
    QVector<ChoosedStantion> GetStantions();

    int YY, MM, DD, h, m, s;

private:
    Ui::WelcomeWindow *ui;
    DBWorker* dbw;

    QVector<PackageSatelliteConstellation> Constellations;
    QVector<PackageSatelliteGroup> Groups;
    QVector<PackageSatellites> Satellites;

    QVector<PackageStantionConstellation> ConstellationsStan;
    QVector<PackageStantions> Stantions;

    QVector<ChoosedSatellite> ChoosedSatellites;
    QVector<ChoosedGroup> ChoosedGroups;
    QVector<ChoosedStantion> ChoosedStantions;

    void PastTableSat();
    void PastTableStan();

    int NChoosedGroup;
    int NChoosedStantion;

signals:
    void openChooseSatellitesWindowRequested();

private slots:
    void on_btnOpenWindowChooseGroup_clicked();
    void ShowTableSat(int);
    void ShowTableStan(int);
};

#endif // WELCOMEWINDOW_H
























