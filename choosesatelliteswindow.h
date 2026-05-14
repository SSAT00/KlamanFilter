#ifndef CHOOSESATELLITESWINDOW_H
#define CHOOSESATELLITESWINDOW_H


#include "UtochnenieSimple/Modules.h"


namespace Ui {
class ChooseSatellitesWindow;
}

class ChooseSatellitesWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ChooseSatellitesWindow(QWidget *parent = nullptr);
    ~ChooseSatellitesWindow();
    void PastSatellites(QVector<ChoosedSatellite>, QVector<ChoosedGroup>);

    QVector<ChoosedSatellite> Satellites;
    QVector<ChoosedGroup> Groups;

    QVector<QCheckBox*> SatellitesCB;
    QVector<QPushButton*> buttons;

    QVector<int> OnOffButtons;
    QVector<ChoosedSatellite> GetSatellites();
    QVector<ChoosedGroup> GetGroups();

private:
    Ui::ChooseSatellitesWindow *ui;
    void ButtonSwitchOffClicked(int);

signals:
    void openVisibilityGroupsWindowRequested();

private slots:
    void on_btnNext_clicked();
};

#endif // CHOOSESATELLITESWINDOW_H
