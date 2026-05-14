#include "welcomewindow.h"
#include "ui_welcomewindow.h"

WelcomeWindow::WelcomeWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WelcomeWindow)
{
    ui->setupUi(this);

    ui->twStantions->horizontalHeader()->setStretchLastSection(true);
    ui->twStantions->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->twStantions->setColumnCount(6);
    QStringList headers = {"Широта, град", "Долгота, град", "Высота, км", "Gmin, град", "Gmax, град", "Описание"};
    ui->twStantions->setHorizontalHeaderLabels(headers);
    ui->twStantions->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->twStantions->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->twStantions->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->twStantions->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->twStantions->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);

    dbw = new DBWorker();
    PastTableSat();
    ShowTableSat(0);

    PastTableStan();
    ShowTableStan(0);

    NChoosedGroup = 0;
    NChoosedStantion = 0;

    connect(ui->cbGroupNames, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &WelcomeWindow::ShowTableSat);

    connect(ui->cbStantions, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &WelcomeWindow::ShowTableStan);
}

WelcomeWindow::~WelcomeWindow()
{
    delete ui;
}

bool WelcomeWindow::UseStan(){
    return !ui->cbNotUseNs->isChecked();
}

void WelcomeWindow::on_btnOpenWindowChooseGroup_clicked() {
    emit openChooseSatellitesWindowRequested();
}

void WelcomeWindow::PastTableSat(){
    Constellations = dbw->GetConstellationsSAT();
    Groups = dbw->GetGroups();
    Satellites = dbw->GetSatellites();

    for(int i = 0; i < Constellations.size(); i++){
        QString Date = QString::number(Constellations[i].YY) + "." + QString::number(Constellations[i].MM) + "." + QString::number(Constellations[i].DD) + " ";
        Date += QString::number(Constellations[i].h) + ":" + QString::number(Constellations[i].m) + ":" + QString::number(Constellations[i].s) + ")";
        QString TableName = Constellations[i].Name + " (" + Date;
        ui->cbGroupNames->addItem(TableName, i);
    }
}

void WelcomeWindow::PastTableStan(){
    ConstellationsStan = dbw->GetConstelationsSTAN();
    Stantions = dbw->GetStantions();

    for(int i = 0; i < ConstellationsStan.size(); i++){
        QString TableName = ConstellationsStan[i].Name;
        ui->cbStantions->addItem(TableName, i);
    }
}

void WelcomeWindow::ShowTableSat(int n_rec){
    while (ui->tlbSatellites->count() > 0) {
        QWidget* page = ui->tlbSatellites->widget(0);
        ui->tlbSatellites->removeItem(0);
        delete page;
    }

    ChoosedSatellites.clear();
    ChoosedGroups.clear();

    YY = Constellations[n_rec].YY;
    MM = Constellations[n_rec].MM;
    DD = Constellations[n_rec].DD;
    h = Constellations[n_rec].h;
    m = Constellations[n_rec].m;
    s = Constellations[n_rec].s;

    for(int i = 0; i < Groups.size(); i++){
        if (Groups[i].NConstellation == Constellations[n_rec].N){
            ChoosedGroup gr;
            gr.N = Groups[i].N;
            gr.Name = Groups[i].Name;
            ChoosedGroups.push_back(gr);

            QWidget* newPage = new QWidget();
            QTableWidget* table = new QTableWidget();
            table->setColumnCount(7);
            table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
            table->horizontalHeader()->setStretchLastSection(true);
            table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            table->setHorizontalHeaderLabels({"X, км", "Y, км", "Z, км", "VX, км/c", "VY, км/c", "VZ, км/c", "Название"});
            table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
            table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
            table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
            table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
            table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
            table->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
            table->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);

            QFont tableFont("Montserrat", 9);
            table->setFont(tableFont);

            for(int j = 0; j < Satellites.size(); j++){
                if (Satellites[j].NGROUP == Groups[i].N){
                    ChoosedSatellite sat;

                    sat.NGROUP = Satellites[j].NGROUP;

                    int currentRow = table->rowCount();
                    table->insertRow(currentRow);

                    QTableWidgetItem *itemName = new QTableWidgetItem(Satellites[j].SatName);
                    itemName->setFlags(itemName->flags() ^ Qt::ItemIsEditable);
                    sat.Name = Satellites[j].SatName;

                    QTableWidgetItem *itemX = new QTableWidgetItem(QString::number(Satellites[j].X.toDouble(), 'f', 2));
                    itemX->setFlags(itemX->flags() ^ Qt::ItemIsEditable);
                    sat.X = Satellites[j].X;

                    QTableWidgetItem *itemY = new QTableWidgetItem(QString::number(Satellites[j].Y.toDouble(), 'f', 2));
                    itemY->setFlags(itemY->flags() ^ Qt::ItemIsEditable);
                    sat.Y = Satellites[j].Y;

                    QTableWidgetItem *itemZ = new QTableWidgetItem(QString::number(Satellites[j].Z.toDouble(), 'f', 2));
                    itemZ->setFlags(itemZ->flags() ^ Qt::ItemIsEditable);
                    sat.Z = Satellites[j].Z;

                    QTableWidgetItem *itemVX = new QTableWidgetItem(QString::number(Satellites[j].VX.toDouble(), 'f', 4));
                    itemVX->setFlags(itemVX->flags() ^ Qt::ItemIsEditable);
                    sat.VX = Satellites[j].VX;

                    QTableWidgetItem *itemVY = new QTableWidgetItem(QString::number(Satellites[j].VY.toDouble(), 'f', 4));
                    itemVY->setFlags(itemVY->flags() ^ Qt::ItemIsEditable);
                    sat.VY = Satellites[j].VY;

                    QTableWidgetItem *itemVZ = new QTableWidgetItem(QString::number(Satellites[j].VZ.toDouble(), 'f', 4));
                    itemVZ->setFlags(itemVZ->flags() ^ Qt::ItemIsEditable);
                    sat.VZ = Satellites[j].VZ;

                    table->setItem(currentRow, 6, itemName);
                    table->setItem(currentRow, 0, itemX);
                    table->setItem(currentRow, 1, itemY);
                    table->setItem(currentRow, 2, itemZ);
                    table->setItem(currentRow, 3, itemVX);
                    table->setItem(currentRow, 4, itemVY);
                    table->setItem(currentRow, 5, itemVZ);

                    sat.A01 = Satellites[j].A01;
                    sat.A02 = Satellites[j].A02;
                    sat.A03 = Satellites[j].A03;
                    sat.A11 = Satellites[j].A11;
                    sat.A12 = Satellites[j].A12;
                    sat.A13 = Satellites[j].A13;
                    sat.B11 = Satellites[j].B11;
                    sat.B12 = Satellites[j].B12;
                    sat.B13 = Satellites[j].B13;
                    sat.Gmin = Satellites[j].Gmin;
                    sat.Gmax = Satellites[j].Gmax;

                    ChoosedSatellites.push_back(sat);
                }
            }

            QVBoxLayout* layout = new QVBoxLayout(newPage);
            layout->addWidget(table);

            QString sectionName = Groups[i].Name;
            ui->tlbSatellites->addItem(newPage, sectionName);
        }
    }
    NChoosedGroup = n_rec;
}

void WelcomeWindow::ShowTableStan(int n_rec){

    ChoosedStantions.clear();

    ui->twStantions->setRowCount(0);
    for(int i = 0; i < Stantions.size(); i++){
        if(Stantions[i].NCONST == Constellations[n_rec].N){
            int currentRow = ui->twStantions->rowCount();
            ui->twStantions->insertRow(currentRow);

            ChoosedStantion stan;

            QTableWidgetItem *itemPhi = new QTableWidgetItem(QString::number(Stantions[i].PHI.toDouble()));
            itemPhi->setFlags(itemPhi->flags() ^ Qt::ItemIsEditable);
            stan.PHI = Stantions[i].PHI;

            QTableWidgetItem *itemLam = new QTableWidgetItem(QString::number(Stantions[i].LAM.toDouble()));
            itemLam->setFlags(itemLam->flags() ^ Qt::ItemIsEditable);
            stan.LAM = Stantions[i].LAM;

            QTableWidgetItem *itemH = new QTableWidgetItem(QString::number(Stantions[i].H.toDouble()));
            itemH->setFlags(itemH->flags() ^ Qt::ItemIsEditable);
            stan.H = Stantions[i].H;

            QTableWidgetItem *itemGmin = new QTableWidgetItem(QString::number(Stantions[i].GMIN.toDouble()));
            itemGmin->setFlags(itemGmin->flags() ^ Qt::ItemIsEditable);
            stan.Gmin = Stantions[i].GMIN;

            QTableWidgetItem *itemGmax = new QTableWidgetItem(QString::number(Stantions[i].GMAX.toDouble()));
            itemGmax->setFlags(itemGmax->flags() ^ Qt::ItemIsEditable);
            stan.Gmax = Stantions[i].GMAX;

            QTableWidgetItem *itemD = new QTableWidgetItem(Stantions[i].Description);
            itemD->setFlags(itemD->flags() ^ Qt::ItemIsEditable);

            ui->twStantions->setItem(currentRow, 0, itemPhi);
            ui->twStantions->setItem(currentRow, 1, itemLam);
            ui->twStantions->setItem(currentRow, 2, itemH);
            ui->twStantions->setItem(currentRow, 3, itemGmin);
            ui->twStantions->setItem(currentRow, 4, itemGmax);
            ui->twStantions->setItem(currentRow, 5, itemD);

            ChoosedStantions.push_back(stan);
        }
    }
    NChoosedStantion = n_rec;
}


QVector<ChoosedGroup> WelcomeWindow::GetGroups(){
    return ChoosedGroups;
}


QVector<ChoosedSatellite> WelcomeWindow::GetSatellites(){
    return ChoosedSatellites;
}


QVector<ChoosedStantion> WelcomeWindow::GetStantions(){
    return ChoosedStantions;
}

























