#include "groupvisibilitywindow.h"
#include "ui_groupvisibilitywindow.h"

GroupVisibilityWindow::GroupVisibilityWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupVisibilityWindow)
{
    ui->setupUi(this);
}

GroupVisibilityWindow::~GroupVisibilityWindow()
{
    delete ui;
}

void GroupVisibilityWindow::PastData(QVector<ChoosedStantion> stan, QVector<ChoosedSatellite> sat, QVector<ChoosedGroup> groups){
    Satellites = sat;
    Groups = groups;
    Stantions = stan;
    fl_stan = true;
    if (Stantions.size() == 0) fl_stan = false;
    PastDataOnFrames2();
}

void GroupVisibilityWindow::PastDataOnFrames2(){
    QGridLayout* grid = new QGridLayout(ui->fSat);
    int nsat = Satellites.size();
    int ngroup = Groups.size();
    int nstan = Stantions.size();

    CyclSat.resize(nsat, nsat);
    CyclSat.setOnes();

    CyclStan.resize(nsat, nstan);
    CyclStan.setOnes();

    QVector<int> sats_in_group;
    for(int i = 0; i < ngroup; i++) sats_in_group.append(0);
    for(int i = 0; i < nsat; i++){
        for(int j = 0; j < ngroup; j++){
            if(Satellites[i].NGROUP == Groups[j].N){
                sats_in_group[j]++;
            }
        }
    }

    QFont Font("Montserrat", 8);
    Font.setWeight(900);

    int k = 0;
    int c;
    for(int i = 0; i < ngroup; i++){
        QLabel* lt = new QLabel();
        lt->setText(" " + Groups[i].Name + " ");
        lt->setStyleSheet("QLabel{border: 1px solid #ccc; border-radius: 5px;}");
        lt->setFont(Font);
        lt->setMaximumHeight(40);
        lt->setMinimumHeight(40);
        lt->setAlignment(Qt::AlignCenter);
        grid->addWidget(lt, 0, k+2, 1, sats_in_group[i]);

        VerticalLabel* lr = new VerticalLabel();
        lr->setText(" " + Groups[i].Name + " ");
        lr->setStyleSheet("QLabel{border: 1px solid #ccc; border-radius: 5px;}");
        lr->setFont(Font);
        lr->setMaximumWidth(40);
        lr->setMinimumWidth(40);
        lr->setAlignment(Qt::AlignCenter);
        grid->addWidget(lr, k+2, 0, sats_in_group[i], 1);

        c = k;
        int p = 0;
        for(int j = 0; j < nsat; j++){
            if(Satellites[j].NGROUP == Groups[i].N){
                QPushButton* btn_t = new QPushButton();
                btn_t->setText(QString::number(p+1));
                btn_t->setFont(Font);
                grid->addWidget(btn_t, 1, k+2+p, 1, 1);

                QPushButton* btn_r = new QPushButton();
                btn_r->setText(QString::number(p+1));
                btn_r->setFont(Font);
                grid->addWidget(btn_r, k+2+p, 1, 1, 1);
                c++;
                p++;
            }
        }

        k += sats_in_group[i];
    }

    if (fl_stan != 0){
        QLabel* lns = new QLabel();
        lns->setText("Станции");
        lns->setStyleSheet("QLabel{border: 1px solid #ccc; border-radius: 5px;}");
        lns->setFont(Font);
        lns->setMaximumHeight(40);
        lns->setMinimumHeight(40);
        lns->setAlignment(Qt::AlignCenter);
        grid->addWidget(lns, 0, k+2, 1, nstan);

    }
    for(int i = 0; i < nstan; i++){
        QPushButton* btn_ns = new QPushButton();
        btn_ns->setText(QString::number(i+1));
        btn_ns->setFont(Font);
        grid->addWidget(btn_ns, 1, k+2+i, 1, 1);
    }

    btns_cycl.resize(nsat);
    for(int i = 0; i < nsat; i++) btns_cycl[i].resize(nsat);

    for(int i = 0 ; i < nsat; i++){
        for(int j = 0; j < nsat; j++){
            QPushButton* btn = new QPushButton();
            btn->setText("");
            if(i == j)btn->setEnabled(false);
            else btn->setStyleSheet("background-color:#76BD66; border:none; border-radius:2px;");
            grid->addWidget(btn, i+2, j+2, 1, 1);
            btns_cycl[i][j] = btn;
        }
    }

    if (nsat < 10){
        QSpacerItem *vSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
        // Добавляем распорку в макет
        grid->addItem(vSpacer, nsat+5, 1);
    }

    for(int i = 0; i < nsat; i ++){
        for(int j = 0; j < nsat; j++){
            connect(btns_cycl[i][j], &QPushButton::clicked, this, [this, i, j]() {
                btn_cycl_click(i, j);
            });
        }
    }
}

Matrix<int, Dynamic, Dynamic> GroupVisibilityWindow::MakeCyclogramSat(){
    return CyclSat;
}

Matrix<int, Dynamic, Dynamic> GroupVisibilityWindow::MakeCyclogramStan(){
    return CyclStan;
}

void GroupVisibilityWindow::on_btnNext_clicked(){
    emit openMainWindowRequested();
}

void GroupVisibilityWindow::btn_cycl_click(int i, int j){
    if (CyclSat(i, j) == 0){
        CyclSat(i, j) = 1;
        btns_cycl[i][j]->setStyleSheet("background-color:#76BD66; border:none; border-radius:2px;");
    }else{
        CyclSat(i, j) = 0;
        btns_cycl[i][j]->setStyleSheet("background-color:#FF8787; border:none; border-radius:2px;");
    }
}







