#include "app.h"


App::App(QObject *parent) : QObject(parent) {
    welcomewin = new WelcomeWindow();
    choosesatwin = new ChooseSatellitesWindow();
    groupvisibilitywin = new GroupVisibilityWindow();
    mainwin = new MainWindow();

    connect(welcomewin, &WelcomeWindow::openChooseSatellitesWindowRequested, this, &App::showOpenChooseSatellitesWindow);
    connect(choosesatwin, &ChooseSatellitesWindow::openVisibilityGroupsWindowRequested, this, &App::showOpenChooseVisibilityGroupWindow);
    connect(groupvisibilitywin, &GroupVisibilityWindow::openMainWindowRequested, this, &App::showOpenMainWindow);
    connect(mainwin, &MainWindow::runKalmanFilter, this, &App::Run);

    QObject::connect(mainwin, &MainWindow::windowClosed, [this]() {
            mainwin->appFilter->stop();
        });

    mainwin->appFilter->sig_setNIters.connect([this](int a, int b) {
        QMetaObject::invokeMethod(mainwin, [this, a, b] {
            mainwin->SetNIters(a, b);
        }, Qt::QueuedConnection);
    });
    mainwin->appFilter->sig_clearNIters.connect([this]() {
        QMetaObject::invokeMethod(mainwin, [this] {
            mainwin->ClearNIters();
        }, Qt::QueuedConnection);
    });

    mainwin->appFilter->sig_setMean.connect([this](double a, int b) {
        QMetaObject::invokeMethod(mainwin, [this, a, b] {
            mainwin->SetMean(a, b);
        }, Qt::QueuedConnection);
    });
    mainwin->appFilter->sig_clearMean.connect([this]() {
        QMetaObject::invokeMethod(mainwin, [this] {
            mainwin->ClearMean();
        }, Qt::QueuedConnection);
    });

    mainwin->appFilter->sig_setSTD.connect([this](double a, int b) {
        QMetaObject::invokeMethod(mainwin, [this, a, b] {
            mainwin->SetSTD(a, b);
        }, Qt::QueuedConnection);
    });
    mainwin->appFilter->sig_clearSTD.connect([this]() {
        QMetaObject::invokeMethod(mainwin, [this] {
            mainwin->ClearSTD();
        }, Qt::QueuedConnection);
    });

    mainwin->appFilter->sig_setCond.connect([this](double a, int b) {
        QMetaObject::invokeMethod(mainwin, [this, a, b] {
            mainwin->SetCond(a, b);
        }, Qt::QueuedConnection);
    });
    mainwin->appFilter->sig_clearCond.connect([this]() {
        QMetaObject::invokeMethod(mainwin, [this] {
            mainwin->ClearCond();
        }, Qt::QueuedConnection);
    });

    mainwin->appFilter->sig_setDqX.connect([this](double a, int b) {
        QMetaObject::invokeMethod(mainwin, [this, a, b] {
            mainwin->SetDqX(a, b);
        }, Qt::QueuedConnection);
    });
    mainwin->appFilter->sig_clearDqX.connect([this]() {
        QMetaObject::invokeMethod(mainwin, [this] {
            mainwin->ClearDqX();
        }, Qt::QueuedConnection);
    });

    mainwin->appFilter->sig_setDqSD.connect([this](double a, int b) {
        QMetaObject::invokeMethod(mainwin, [this, a, b] {
            mainwin->SetDqSD(a, b);
        }, Qt::QueuedConnection);
    });
    mainwin->appFilter->sig_clearDqSD.connect([this]() {
        QMetaObject::invokeMethod(mainwin, [this] {
            mainwin->ClearDqSD();
        }, Qt::QueuedConnection);
    });

    mainwin->appFilter->sig_setNObs.connect([this](int a, int b) {
        QMetaObject::invokeMethod(mainwin, [this, a, b] {
            mainwin->SetNObs(a, b);
        }, Qt::QueuedConnection);
    });
    mainwin->appFilter->sig_clearNObs.connect([this]() {
        QMetaObject::invokeMethod(mainwin, [this] {
            mainwin->ClearNObs();
        }, Qt::QueuedConnection);
    });

    mainwin->appFilter->sig_setLocalProgress.connect([this](int a, QString b, QString c) {
        QMetaObject::invokeMethod(mainwin, [this, a, b, c] {
            mainwin->SetLocalProgress(a, b, c);
        }, Qt::QueuedConnection);
    });
    mainwin->appFilter->sig_setProgress.connect([this](int a, QString b) {
        QMetaObject::invokeMethod(mainwin, [this, a, b] {
            mainwin->SetProgress(a, b);
        }, Qt::QueuedConnection);
    });
}

void App::start() {
    welcomewin->show();
}

void App::showOpenChooseSatellitesWindow() {

    Satellites = welcomewin->GetSatellites();
    Groups = welcomewin->GetGroups();
    Stantions = welcomewin->GetStantions();

    bool fl = welcomewin->UseStan();
    if (!fl) Stantions.clear();

    welcomewin->close();

    choosesatwin->show();
    choosesatwin->PastSatellites(Satellites, Groups);
}

void App::showOpenChooseVisibilityGroupWindow(){

    Satellites = choosesatwin->GetSatellites();
    Groups = choosesatwin->GetGroups();

    choosesatwin->close();

    groupvisibilitywin->PastData(Stantions, Satellites, Groups);
    groupvisibilitywin->show();
}

void App::PrepareDataForStart(){
    nsat = Satellites.size();
    nstan = Stantions.size();

    SatCoords.resize(nsat, 6);
    for(int i = 0; i < nsat; i++){
        SatCoords(i, 0) = stold(Satellites[i].X.toStdString());
        SatCoords(i, 1) = stold(Satellites[i].Y.toStdString());
        SatCoords(i, 2) = stold(Satellites[i].Z.toStdString());
        SatCoords(i, 3) = stold(Satellites[i].VX.toStdString());
        SatCoords(i, 4) = stold(Satellites[i].VY.toStdString());
        SatCoords(i, 5) = stold(Satellites[i].VZ.toStdString());
    }

    StanCoords.resize(nstan, 3);
    for(int i = 0; i < nstan; i++){
        long double PHI, LAM, H, X, Y, Z;
        PHI = stold(Stantions[i].PHI.toStdString());
        LAM = stold(Stantions[i].LAM.toStdString());
        H = stold(Stantions[i].H.toStdString());
        X = H * cos(PHI) * cos(LAM);
        Y = H * cos(PHI) * sin(LAM);
        Z = H * sin(PHI);
        StanCoords(i, 0) = X;
        StanCoords(i, 1) = Y;
        StanCoords(i, 2) = Z;
    }

    SatNames.resize(nsat);
    for(int i = 0; i < nsat; i ++){
        SatNames(i) = Satellites[i].Name;
    }

    lpc.resize(nsat, 9);
    for(int i = 0; i < nsat; i ++){
        lpc(i, 0) = stold(Satellites[i].A01.toStdString());
        lpc(i, 1) = stold(Satellites[i].A02.toStdString());
        lpc(i, 2) = stold(Satellites[i].A03.toStdString());
        lpc(i, 3) = stold(Satellites[i].A11.toStdString());
        lpc(i, 4) = stold(Satellites[i].A12.toStdString());
        lpc(i, 5) = stold(Satellites[i].A13.toStdString());
        lpc(i, 6) = stold(Satellites[i].B11.toStdString());
        lpc(i, 7) = stold(Satellites[i].B12.toStdString());
        lpc(i, 8) = stold(Satellites[i].B13.toStdString());
    }

    angles_sat.resize(nsat, 2);
    for(int i = 0; i < nsat; i++){
        angles_sat(i, 0) = stold(Satellites[i].Gmin.toStdString());
        angles_sat(i, 1) = stold(Satellites[i].Gmax.toStdString());
    }

    angles_stan.resize(nstan, 2);
    for(int i = 0; i < nstan; i++){
        angles_stan(i, 0) = stold(Stantions[i].Gmin.toStdString());
        angles_stan(i, 1) = stold(Stantions[i].Gmax.toStdString());
    }

    for(int i = 0; i < Groups.size(); i++){
        GroupsNames.append(Groups[i].Name);
        GroupsNum.append(Groups[i].N);
    }
    for(int i = 0; i < Satellites.size(); i++){
        SatsNGroup.append(Satellites[i].NGROUP);
    }
}

void App::showOpenMainWindow(){

    cycl_sat = groupvisibilitywin->MakeCyclogramSat();
    cycl_stan = groupvisibilitywin->MakeCyclogramStan();

    PrepareDataForStart();

    groupvisibilitywin->close();

    mainwin->show();

    int YY, MM, DD, h, m, s;
    YY = welcomewin->YY;
    MM = welcomewin->MM;
    DD = welcomewin->DD;
    h = welcomewin->h;
    m = welcomewin->m;
    s = welcomewin->s;

    QDateTime dtStart = QDateTime(QDate(YY, MM, DD), QTime(h, m, s));
    mainwin->SetDataStart(dtStart);
}

void App::Run(){
    SetData2App();
    QtConcurrent::run([this]() {
        mainwin->appFilter->RunApp();
    });
    mainwin->CloseStartFrames();
}

void App::SetData2App(){
    mainwin->appFilter->SetNSAT(nsat);
    mainwin->appFilter->SetNSTAN(nstan);

    mainwin->appFilter->SetSatCoords(SatCoords);
    mainwin->appFilter->SetStanCoords(StanCoords);

    mainwin->appFilter->SetSatNames(SatNames);

    mainwin->appFilter->SetSatAngle(angles_sat);
    mainwin->appFilter->SetStanAngle(angles_stan);

    mainwin->appFilter->SetLPC(lpc);

    int NM, MN;
    bool sun, moon, plans, lp, rel, tides;
    mainwin->GetPerts(NM, MN, sun, moon, plans, lp, rel, tides);

    mainwin->appFilter->SetPerturb(NM, MN, sun, moon, plans, lp, rel, tides); // Сделать форму на окне

    long double dt, block, gap, interval;
    mainwin->GetDtParams(dt, block, gap, interval);

    mainwin->appFilter->SetTimeNet(dt, block, gap, interval);

    int YY, MM, DD, h, m, s;
    YY = welcomewin->YY;
    MM = welcomewin->MM;
    DD = welcomewin->DD;
    h = welcomewin->h;
    m = welcomewin->m;
    s = welcomewin->s;

    QDateTime dt_p = QDateTime(QDate(YY, MM, DD), QTime(h, m, s));
    QDateTime dt_f = mainwin->GetPataFinish();

    mainwin->appFilter->SetNavigationFrames(dt_p, dt_f);

    mainwin->appFilter->SetCyclogramSatSat(cycl_sat);

    mainwin->appFilter->SetCyclogramSatStan(cycl_stan);

    int npars = mainwin->GetNParams();
    mainwin->appFilter->SetNParamsRefine(npars);

    long double NoiseSat, NoiseStan, SysErr;
    mainwin->GetNoises(NoiseSat, NoiseStan, SysErr);
    mainwin->appFilter->SetNoises(NoiseSat, NoiseStan, SysErr);

    long double dempf;
    mainwin->GetDempf(dempf);
    mainwin->appFilter->SetDempf(dempf);

    long double acc;
    mainwin->GetAcc(acc);
    mainwin->appFilter->SetAcc(acc);

    mainwin->SetSatGroups(GroupsNames, SatsNGroup, SatNames, GroupsNum);
}

























