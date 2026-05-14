#include "choosesatelliteswindow.h"
#include "ui_choosesatelliteswindow.h"

ChooseSatellitesWindow::ChooseSatellitesWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChooseSatellitesWindow)
{
    ui->setupUi(this);
}

ChooseSatellitesWindow::~ChooseSatellitesWindow()
{
    delete ui;
}

void ChooseSatellitesWindow::on_btnNext_clicked(){
    emit openVisibilityGroupsWindowRequested();
}

void ChooseSatellitesWindow::PastSatellites(QVector<ChoosedSatellite> sats, QVector<ChoosedGroup> groups){
    Satellites = sats;
    Groups = groups;
    while (ui->tlbChoosedSatellites->count() > 0) {
        QWidget* page = ui->tlbChoosedSatellites->widget(0);
        ui->tlbChoosedSatellites->removeItem(0);
        delete page;
    }
    QFont Font("Montserrat", 8);
    for(int i = 0; i < Groups.size(); i++){
        OnOffButtons.push_back(1);
        QWidget* newPage = new QWidget();

        QGridLayout* prelayout = new QGridLayout(newPage);

        QFrame* preframe = new QFrame();

        QPushButton* btn_switchoff = new QPushButton();
        btn_switchoff->setText("Выключить все");
        btn_switchoff->setMinimumSize(140, 40);
        btn_switchoff->setMaximumSize(140, 40);
        Font.setWeight(700);
        btn_switchoff->setFont(Font);
        Font.setWeight(900);

        prelayout->addWidget(preframe, 0, 0, 1, 3);
        prelayout->addWidget(btn_switchoff, 1, 1);

        buttons.push_back(btn_switchoff);

        int k1 = 0, k2 = 0;
        QGridLayout* layout = new QGridLayout(preframe);
        for(int j = 0; j < Satellites.size(); j++){
            if(Groups[i].N == Satellites[j].NGROUP){
                QCheckBox* cb = new QCheckBox();
                cb->setText(Satellites[j].Name);
                cb->setChecked(true);
                cb->setFont(Font);
                layout->addWidget(cb, k1, k2);
                SatellitesCB.append(cb);
                k2++;
                if(k2 == 5){
                    k2 = 0;
                    k1++;
                }
            }
        }

        QSpacerItem* si = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
        k1++;
        layout->addItem(si, k1, 0);
        layout->setContentsMargins(40, 40, 40, 40);
        layout->setVerticalSpacing(20);
        layout->setHorizontalSpacing(20);

        QString sectionName = Groups[i].Name;
        newPage->setFont(Font);
        ui->tlbChoosedSatellites->addItem(newPage, sectionName);
    }
    for (int i = 0; i < buttons.size(); ++i) {
        connect(buttons[i], &QPushButton::clicked, [this, i]() {
            ButtonSwitchOffClicked(i);
        });
    }
}

void ChooseSatellitesWindow::ButtonSwitchOffClicked(int n){
    int ngroup = Groups[n].N;
    if (OnOffButtons[n] == 1){
        for(int i = 0; i < Satellites.size(); i++){
            if (Satellites[i].NGROUP == ngroup) SatellitesCB[i]->setChecked(false);
        }
        OnOffButtons[n] = 0;
        buttons[n]->setText("Включить все");
    }else{
        for(int i = 0; i < Satellites.size(); i++){
            if (Satellites[i].NGROUP == ngroup) SatellitesCB[i]->setChecked(true);
        }
        OnOffButtons[n] = 1;
        buttons[n]->setText("Выключить все");
    }
}

QVector<ChoosedSatellite> ChooseSatellitesWindow::GetSatellites(){
    QVector<ChoosedSatellite> SatellitesNext;

    for(int i = 0; i < Satellites.size(); i++){
        if (SatellitesCB[i]->isChecked()){
            SatellitesNext.push_back(Satellites[i]);
        }
    }
    return SatellitesNext;
}

QVector<ChoosedGroup> ChooseSatellitesWindow::GetGroups(){
    QVector<ChoosedGroup> GroupsNext;
    QVector<int> ChoosedGroups;

    for(int i = 0; i < Groups.size(); i++) ChoosedGroups.push_back(0);

    for(int i = 0; i < Satellites.size(); i++){
        if (SatellitesCB[i]->isChecked()){
            for(int j = 0; j < Groups.size(); j++){
                if(Satellites[i].NGROUP == Groups[j].N){
                    ChoosedGroups[j] = 1;
                }
            }
        }
    }

    for(int i = 0; i < Groups.size(); i++){
        if(ChoosedGroups[i] != 0){
            GroupsNext.append(Groups[i]);
        }
    }
    return GroupsNext;
}


















