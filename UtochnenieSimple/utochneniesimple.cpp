#include "utochneniesimple.h"


void UtochnenieSimple::NormalizeQG(int n, Matrix<long double, Dynamic, Dynamic>& Q, Matrix<long double, Dynamic, 1>& G){
    Matrix<long double, 1, 15> scales = {sclx, sclx, sclx, sclv, sclv, sclv, scllc, scllc, scllc, scllc, scllc, scllc, scllc, scllc, scllc};
    for(int i = 0; i < n; i++){
        G(i) = G(i) * scales(i);
        for(int j = 0; j < n; j++){
            Q(i, j) = Q(i, j) * scales(i) * scales(j);
        }
    }
}

void UtochnenieSimple::UnNormalizeDq(int n, Matrix<long double, Dynamic, 1>& dq){
    Matrix<long double, 1, 15> scales = {sclx, sclx, sclx, sclv, sclv, sclv, scllc, scllc, scllc, scllc, scllc, scllc, scllc, scllc, scllc};
    for(int i = 0; i < n; i++){
        dq(i) = dq(i) * scales(i);
    }
}

UtochnenieSimple::UtochnenieSimple()
{
    sclx = 1e5;
    sclv = 1e0;
    scllc = 1e1;
}

void UtochnenieSimple::SetNSAT(int n){
    NSAT = n;

    satelliteGenerators.resize(NSAT);

    for (int i = 0; i < NSAT; ++i) {
        satelliteGenerators[i].seed(i);
        satellite sat;
        satellites.push_back(sat);
    }
}

void UtochnenieSimple::SetNSTAN(int n){
    NSTAN = n;
    for(int i = 0; i < NSTAN; i++){
        stantion ns;
        stantions.push_back(ns);
    }
}

void UtochnenieSimple::SetSatCoords(Matrix<long double, Dynamic, 6> coords){
    for(int i = 0; i < NSAT; i++){
        for(int j = 0; j < 6; j++){
            satellites[i].XE0(j) = coords(i, j);
            satellites[i].XC0(j) = coords(i, j);
        }
    }
}

void UtochnenieSimple::SetStanCoords(Matrix<long double, Dynamic, 3> coords){
    for(int i = 0; i < NSTAN; i++){
        stantions[i].X = coords(i, 0);
        stantions[i].Y = coords(i, 1);
        stantions[i].Z = coords(i, 2);
    }
}

void UtochnenieSimple::SetSatNames(Matrix<QString, Dynamic, 1> names){
    for(int i = 0; i < NSAT; i++){
        satellites[i].name = names(i).toStdString();
    }
}

void UtochnenieSimple::SetLPC(Matrix<long double, Dynamic, 9> lpc){
    for(int i = 0; i < NSAT; i ++){
        for(int j = 0; j < 9; j++) {
            satellites[i].LightCoeffs(j) = lpc(i, j);
            satellites[i].LightCoeffs_ref(j) = lpc(i, j);
        }
    }
}

void UtochnenieSimple::SetSatAngle(Matrix<long double, Dynamic, 2> angs){
    for(int i = 0; i < NSAT; i++){
        satellites[i].GMIN = angs(i, 0);
        satellites[i].GMAX = angs(i, 1);
    }
}

void UtochnenieSimple::SetStanAngle(Matrix<long double, Dynamic, 2> angs){
    for(int i = 0; i < NSTAN; i++){
        stantions[i].GMIN = angs(i, 0);
        stantions[i].GMAX = angs(i, 1);
    }
}

void UtochnenieSimple::SetCyclogramSatSat(Matrix<int, Dynamic, Dynamic> cycl){
    for(int i = 0; i < NSAT; i++){
        satellites[i].Cyclogram.resize(NSAT);
        for(int j = 0; j < NSAT; j++){
            satellites[i].Cyclogram(j) = cycl(i, j);
        }
    }
}

void UtochnenieSimple::SetCyclogramSatStan(Matrix<int, Dynamic, Dynamic> cycl){
    for(int i = 0; i < NSAT; i++){
        satellites[i].CyclogramStan.resize(NSTAN);
        for(int j = 0; j < NSTAN; j++){
            satellites[i].CyclogramStan(j) = cycl(i, j);
        }
    }
}

void UtochnenieSimple::SetNoises(long double sat, long double stan, long double sys){
    NoiseSat = sat;
    NoiseStan = stan;
    SysErr = sys;
    SysErrSat.resize(NSAT, NSAT);
    SysErrStan.resize(NSAT, NSTAN);
    SysErrSat.setZero();
    SysErrStan.setZero();
    if (SysErr != 0.0){
        std::random_device rd;
        std::mt19937_64 gen(rd());
        std::normal_distribution<long double> dist(0.0, SysErr);
        for(int i = 0; i < NSAT; i ++){
            for(int j = 0 ; j < NSAT; j++){
                SysErrSat(i, j) = dist(gen);
            }
            for(int j = 0; j < NSTAN; j ++){
                SysErrStan(i, j) = dist(gen);
            }
        }
    }
}

string UtochnenieSimple::cleanLine(string line) {
    size_t pos = line.find(" - ");
    if (pos != std::string::npos) {
        return line.substr(0, pos);
    }
    return line;
}

long double UtochnenieSimple::QDate2Julian(QDateTime dt){
    long long jd_integer = dt.date().toJulianDay();
    double dayFraction = (dt.time().hour() * 3600.0 +
                          dt.time().minute() * 60.0 +
                          dt.time().second()) / 86400.0;
    long double fullJD = jd_integer - 0.5 + dayFraction;
    return fullJD;
}

long double UtochnenieSimple::Sid2000(long double djd){
    const long double PI2 = 6.283185307179586476925286766559L;
    const long double S2R = 7.272205216643039903848711535369E-5L;

    long double ut1 = djd - 2400000.5L;
    long double tu = (ut1 - 51544.5L) / 36525.0L;

    // Дробная часть текущих суток в радианах
    long double dayFraction = (ut1 - std::floor(ut1)) * PI2;

    // Вековая часть в секундах, переведенная в радианы через S2R
    long double secular = (24110.54841L + (8640184.812866L +
                          (0.093104L - 6.2e-6L * tu) * tu) * tu) * S2R;

    long double s = std::fmod(dayFraction + secular, PI2);

    if (s < 0) s += PI2;
    return s;
}

void UtochnenieSimple::SetTimeNet(long double dt, long double block, long double gap, long double interval){
    DT = dt;
    BLOCK = block;
    GAP = gap;
    INTERVAL = interval;
    SEANS = block + gap;

    NM = INTERVAL / DT;
    NU = SEANS / DT;

    for(int i1 = 0; i1 < NSAT; i1++){
        satellites[i1].Derivatives.resize(NM, 45);
        satellites[i1].EPH_E.resize(NM, 6);
        satellites[i1].EPH_C.resize(NM, 6);
        satellites[i1].EPH_X.resize(NM, 6);
        satellites[i1].EPH_T_E.resize(NM, 1);
        satellites[i1].EPH_T_C.resize(NM, 1);
        satellites[i1].OBS.resize(NM, 1);
        satellites[i1].OBS_STAN.resize(NM, 1);
        for(int j1 = 0; j1 < NM; j1++) satellites[i1].OBS(j1).resize(NSAT, 4);
        for(int j1 = 0; j1 < NM; j1++) satellites[i1].OBS_STAN(j1).resize(NSTAN, 4);
        satellites[i1].UsageObs.resize(NM, NSAT);
        satellites[i1].UsageObs_STAN.resize(NM, NSTAN);
    }
    for(int i1 = 0; i1 < NSTAN; i1++){
        stantions[i1].EPH_E.resize(NM, 3);
        stantions[i1].EPH_T_E.resize(NM, 1);
    }
}

void UtochnenieSimple::SetNavigationFrames(QDateTime dt_s_, QDateTime dt_f_){
    dt_s = dt_s_;
    dt_f = dt_f_;
    dt_p = dt_s_;

    NSES = (dt_s.secsTo(dt_f) + INTERVAL) / SEANS;
}

void UtochnenieSimple::SetPerturb(int geo_nm, int geo_mn, bool sun, bool moon, bool planets, bool rad, bool rel_eff, bool tides){
    GeoNM = geo_nm;
    GeoMN = geo_mn;
    pertMoon = moon;
    pertSun = sun;
    pertPlanets = planets;
    pertTides = tides;
    pertRadiation = rad;
    pertRelatEffects = rel_eff;
}

void UtochnenieSimple::SetNParamsRefine(int n){
    NPAR = n;
}

input_data_propagation_t UtochnenieSimple::GetInputDataProp(QDateTime dt_s_, QDateTime dt_f_, int n_sat, bool fl_ref){
    input_data_propagation_t input_gln;
    Matrix<long double, 6, 1> q;
    if(fl_ref) for(int i1 = 0; i1 < 6; i1++) q(i1) = satellites[n_sat].XE0(i1);
    else for(int i1 = 0; i1 < 6; i1++) q(i1) = satellites[n_sat].XC0(i1);

    input_gln.GeoNM = GeoNM;
    input_gln.GeoMN = GeoMN;
    input_gln.pertTides = pertTides;
    input_gln.pertSun = pertSun;
    input_gln.pertMoon = pertMoon;
    input_gln.pertPlanets = pertPlanets;
    input_gln.pertRadiation = pertRadiation;
    input_gln.pertRelatEffects = pertRelatEffects;

    input_gln.ts = dt_s_;
    input_gln.tf = dt_f_;
    input_gln.t0 = dt_p;
    input_gln.step = DT;

    QVector<long double> x(48);
    for(int i = 0; i < 48; i++) x[i] = 0.0L;
    for(int i = 0; i < 3; i++) x[i] = q(i);
    x[3] = x[19] = x[35] = 1.0;

    QVector<long double> y(48);
    for(int i = 0; i < 48; i++) y[i] = 0.0L;
    for(int i = 3; i < 6; i++) y[i-3] = q(i);
    y[6] = y[22] = y[38] = 1.0;

    input_gln.x = x;
    input_gln.y = y;

    input_gln.integrOrder = 12;
    input_gln.integrStep = 1000.0;
    input_gln.integrEtol = 1e-12;

    QVector<long double> lightCoef(9);
    if(fl_ref)for (int i = 0; i < 9; i ++) lightCoef[i] = satellites[n_sat].LightCoeffs_ref(i);
    else for (int i = 0; i < 9; i ++) lightCoef[i] = satellites[n_sat].LightCoeffs(i);
    input_gln.lightCoef = lightCoef;

    return input_gln;
}

Propagation UtochnenieSimple::GetPropagationObj(){
    Propagation gln;
    gln.setFileEop("nm/eop/EOP.DAT");
    gln.setFileFond("nm/fond/fond438.bin");
    return gln;
}

void UtochnenieSimple::PropagateRef(int n_sat){
    long double sec_btw_par_and_start = dt_s.secsTo(dt_p);
    QDateTime start_prop = dt_p.addSecs(-DT * (KF-1));
    QDateTime dt_p_ = dt_p;
    input_data_propagation_t input_gln = GetInputDataProp(start_prop, dt_p_, n_sat, true);
    Propagation gln = GetPropagationObj();
    gln.init(input_gln);
    gln.calculate();
    QVector<ephemeris_t> eph = gln.getEphemeris();
    for(int K = 0; K < KF; K++){
        for(int i1 = 0; i1 < 6; i1 ++) satellites[n_sat].EPH_E(K, i1) = eph[K].pos[i1];
        satellites[n_sat].EPH_T_E(K) = sec_btw_par_and_start + eph[K].time;
    }
}

void UtochnenieSimple::PropagateFirst(){
    cout << "Start propagation." << endl;
    auto start = std::chrono::high_resolution_clock::now();
    for(int n_sat = 0; n_sat < NSAT; n_sat++){
        long double sec_btw_par_and_start = dt_s.secsTo(dt_p);
        QDateTime start_prop = dt_p.addSecs(-DT * (KF - 1));
        QDateTime dt_p_ = dt_p;
        input_data_propagation_t input_gln = GetInputDataProp(start_prop, dt_p_, n_sat, true);
        Propagation gln = GetPropagationObj();
        gln.init(input_gln);
        gln.calculate();
        QVector<ephemeris_t> eph = gln.getEphemeris();
        for(int K = 0; K < KF; K++){
            for(int i1 = 0; i1 < 6; i1 ++) satellites[n_sat].EPH_E(K, i1) = eph[K].pos[i1];
            satellites[n_sat].EPH_T_E(K) = sec_btw_par_and_start + eph[K].time;
            for(int i1 = 0; i1 < 6; i1 ++) satellites[n_sat].EPH_C(K, i1) = eph[K].pos[i1];
            satellites[n_sat].EPH_T_C(K) = sec_btw_par_and_start + eph[K].time;
        }
        cout << satellites[n_sat].name << endl;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    cout << fixed << setprecision(3) << "Finish propagation. " << duration.count() << " second." << endl;
    cout << endl;
}

void UtochnenieSimple::PropagateDef(int n_sat){
    long double sec_btw_par_and_start = dt_s.secsTo(dt_p);
    QDateTime start_prop = dt_p.addSecs(-DT * (KF - 1));
    QDateTime dt_p_ = dt_p;
    input_data_propagation_t input_gln = GetInputDataProp(start_prop, dt_p_, n_sat, false);
    Propagation gln = GetPropagationObj();
    gln.init(input_gln);
    gln.calculate();
    QVector<ephemeris_t> eph = gln.getEphemeris();
    for(int K = 0; K < KF; K++){
        for(int i1 = 0; i1 < 6; i1 ++) satellites[n_sat].EPH_C(K, i1) = eph[K].pos[i1];
        satellites[n_sat].EPH_T_C(K) = sec_btw_par_and_start + eph[K].time;
    }
}

void UtochnenieSimple::PropagateXREfining(int n_sat){
    QDateTime start_prop = dt_p.addSecs(-DT * (NM - 1));
    QDateTime dt_p_ = dt_p;
    input_data_propagation_t input_gln = GetInputDataProp(start_prop, dt_p_, n_sat, false);
    Propagation gln = GetPropagationObj();
    gln.init(input_gln);
    gln.calculate();
    QVector<ephemeris_t> eph = gln.getEphemeris();
    for(int K = 0; K < NM; K++){
        for(int i1 = 0; i1 < 6; i1 ++) satellites[n_sat].EPH_X(K, i1) = eph[K].pos[i1];
        for(int i1 = 0; i1 < 45; i1++) satellites[n_sat].Derivatives(K, i1) = eph[K].derivatives_x[i1];
    }
}

void UtochnenieSimple::PropagateStantion(){
    Matrix<long double, 3, 3> A;
    Matrix<long double, 3, 1> rv, rv_jd;
    long double sec_btw_par_and_start = dt_s.secsTo(dt_p);
    A.setZero();
    A(2, 2) = 1;
    long double jd, sid2000;
    QDateTime dt_mom;
    long double cos_s, sin_s;
    for(int K = 0; K < KF; K++){
        dt_mom = dt_p.addSecs(- K * DT);
        jd = QDate2Julian(dt_mom);
        sid2000 = Sid2000(jd);
        cos_s = cos(sid2000);
        sin_s = sin(sid2000);
        A(0, 0) = cos_s;
        A(0, 1) = -sin_s;
        A(1, 0) = sin_s;
        A(1, 1) = cos_s;
        for(int i = 0; i < NSTAN; i++){
            rv = {stantions[i].X, stantions[i].Y, stantions[i].Z};
            rv_jd = A * rv;
            stantions[i].EPH_E(K, 0) = rv_jd(0);
            stantions[i].EPH_E(K, 1) = rv_jd(1);
            stantions[i].EPH_E(K, 2) = rv_jd(2);
            stantions[i].EPH_T_E(K) = sec_btw_par_and_start + dt_p.secsTo(dt_mom);
        }
    }
}

void UtochnenieSimple::Propagate(){
    cout << "Start propagation." << endl;
    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < NSAT; i ++){
        PropagateDef(i);
        PropagateRef(i);
        cout << satellites[i].name << endl;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    cout << fixed << setprecision(3) << "Finish propagation. " << duration.count() << " second." << endl;
    cout << endl;
}

void UtochnenieSimple::ObservateSat(int n_sat){
    long double mean = 0.0;
    normal_distribution<long double> dist(mean, NoiseSat);
    normal_distribution<long double> dist_stan(mean, NoiseStan);
    long double stoch_noise, sys_noise;
    Matrix<double, 3, 1> r_r, r_t;
    long double x_r, x_t, x_t_, y_r, y_t, y_t_, z_r, z_t, z_t_, d_o;
    for(int j = 0; j < NSAT; j++){
        if (n_sat != j){
            for(int K = 0; K < KF; K++){
                x_r = satellites[n_sat].EPH_E(K, 0);
                y_r = satellites[n_sat].EPH_E(K, 1);
                z_r = satellites[n_sat].EPH_E(K, 2);

                x_t = satellites[j].EPH_E(K, 0);
                y_t = satellites[j].EPH_E(K, 1);
                z_t = satellites[j].EPH_E(K, 2);

                x_t_ = satellites[j].EPH_C(K, 0);
                y_t_ = satellites[j].EPH_C(K, 1);
                z_t_ = satellites[j].EPH_C(K, 2);

                stoch_noise = dist(satelliteGenerators[n_sat]);
                sys_noise = SysErrSat(n_sat, j);

                d_o = sqrt(pow(x_r-x_t, 2) + pow(y_r-y_t, 2) + pow(z_r - z_t, 2)) + (stoch_noise + sys_noise) / 100000.0;

                satellites[n_sat].OBS(K)(j, 0) = x_t_;
                satellites[n_sat].OBS(K)(j, 1) = y_t_;
                satellites[n_sat].OBS(K)(j, 2) = z_t_;
                satellites[n_sat].OBS(K)(j, 3) = d_o;

                r_r = {x_r, y_r, z_r};
                r_t = {x_t, y_t, z_t};
                if(ObsInBlock(K, n_sat) && satellites[n_sat].Cyclogram(j)){
                    satellites[n_sat].UsageObs(K, j) = SatellitesVisible(r_r, r_t, n_sat);
                 }else{
                    satellites[n_sat].UsageObs(K, j) = 0;
                }
            }
        }
    }
    for(int j = 0; j < NSTAN; j++){
        for(int K = 0; K < KF; K++){
            x_r = satellites[n_sat].EPH_E(K, 0);
            y_r = satellites[n_sat].EPH_E(K, 1);
            z_r = satellites[n_sat].EPH_E(K, 2);

            x_t = stantions[j].EPH_E(K, 0);
            y_t = stantions[j].EPH_E(K, 1);
            z_t = stantions[j].EPH_E(K, 2);

            stoch_noise = dist_stan(satelliteGenerators[n_sat]);
            sys_noise = SysErrStan(n_sat, j);

            d_o = sqrt(pow(x_r-x_t, 2) + pow(y_r-y_t, 2) + pow(z_r - z_t, 2)) + (stoch_noise + sys_noise) / 100000.0;

            satellites[n_sat].OBS_STAN(K)(j, 0) = x_t;
            satellites[n_sat].OBS_STAN(K)(j, 1) = y_t;
            satellites[n_sat].OBS_STAN(K)(j, 2) = z_t;
            satellites[n_sat].OBS_STAN(K)(j, 3) = d_o;

            r_r = {x_r, y_r, z_r};
            r_t = {x_t, y_t, z_t};

            if(satellites[n_sat].CyclogramStan(j)){
                satellites[n_sat].UsageObs_STAN(K, j) = StantionsVisible(r_r, r_t, j);
            } else satellites[n_sat].UsageObs_STAN(K, j) = 0;
        }
    }
}

void UtochnenieSimple::Observate(){
    cout << "Start observating." << endl;
    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < NSAT; i ++){
        ObservateSat(i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    cout << fixed << setprecision(3) << "Finish observating. " << duration.count() << " second." << endl;
    cout << endl;
}

void UtochnenieSimple::WriteObsSat(int n_sat){
    std::ofstream file_obs;
    file_obs.open("ResultsCoords/OBS/"+satellites[n_sat].name+".txt", std::ios::app);
    for(int K = NM-1; K >= 0; K--){
        for(int j = 0; j < NSAT; j++){
            if(j != n_sat){
                file_obs << fixed << setprecision(5) << satellites[n_sat].EPH_T_E(K) << " ";
                file_obs << satellites[j].name << " ";
                file_obs << satellites[n_sat].OBS(K)(j, 3) << " ";
                file_obs << satellites[n_sat].UsageObs(K, j) << endl;
            }
        }
    }
    file_obs.close();
}

bool UtochnenieSimple::ObsInBlock(int K, int n_sat){
    long double T = satellites[n_sat].EPH_T_E(K);

    if (T == 0) return 1;

    if (T < 0) T += INTERVAL;

    long double fmodt = fmod(T, SEANS);

    if (fmodt <= BLOCK || abs(fmodt-BLOCK) < 1e-5) return 1;

    return 0;
}

QVector<double> UtochnenieSimple::RefineSat(int n_sat){
    Matrix<long double, Dynamic, Dynamic> Q;
    Q.resize(NPAR, NPAR);
    Matrix<long double, Dynamic, 1> G;
    G.resize(NPAR, 1);
    Matrix<long double, 1, 3> dr_dx;
    long double x, y, z;
    long double d_c;
    long double d_o, x_t, y_t, z_t;
    Matrix<long double, 3, Dynamic> dx_need;
    dx_need.resize(3, NPAR);
    Matrix<long double, 1, Dynamic> A;
    A.resize(1, NPAR);
    Matrix<long double, Dynamic, 1> dq, AT;
    dq.resize(NPAR, 1);
    AT.resize(NPAR, 1);
    int n_iters = 0;
    QVector<double> d_o_d_c;
    double acc = 1e-7;
    long double ndq = 1;

    while(acc < ndq){

        PropagateXREfining(n_sat);

        n_iters += 1;
        Q.setZero();
        G.setZero();

        for(int K = 0; K < NM; K++){
            x = satellites[n_sat].EPH_X(K, 0);
            y = satellites[n_sat].EPH_X(K, 1);
            z = satellites[n_sat].EPH_X(K, 2);
            for (int i1 = 0; i1 < 3; i1++){
                for(int j1 = 0; j1 < NPAR; j1++){
                    dx_need(i1, j1) = satellites[n_sat].Derivatives(K, i1*15+j1);
                }
            }
            for (int j = 0; j < NSAT; j++){
                if (n_sat != j && satellites[n_sat].UsageObs(K, j) == 1){
                    x_t = satellites[n_sat].OBS(K)(j, 0);
                    y_t = satellites[n_sat].OBS(K)(j, 1);
                    z_t = satellites[n_sat].OBS(K)(j, 2);
                    d_o = satellites[n_sat].OBS(K)(j, 3);
                    d_c = sqrt(pow(x-x_t, 2) + pow(y-y_t, 2) + pow(z-z_t, 2));
                    dr_dx = {(x-x_t)/d_c, (y-y_t)/d_c, (z-z_t)/d_c}; // Производные вычисленной дальности по координатам
                    A = dr_dx * dx_need;
                    AT = A.transpose();
                    Q += AT * A; // Заполнение матрицы нормальных уравнений
                    G += AT * (d_o - d_c); // Заполнение градиента
                    d_o_d_c.append(d_o - d_c);
                }
            }
            for(int j = 0; j < NSTAN; j++){
                if(satellites[n_sat].UsageObs_STAN(K, j) == 1){
                    x_t = satellites[n_sat].OBS_STAN(K)(j, 0);
                    y_t = satellites[n_sat].OBS_STAN(K)(j, 1);
                    z_t = satellites[n_sat].OBS_STAN(K)(j, 2);
                    d_o = satellites[n_sat].OBS_STAN(K)(j, 3);
                    d_c = sqrt(pow(x-x_t, 2) + pow(y-y_t, 2) + pow(z-z_t, 2));
                    dr_dx = {(x-x_t)/d_c, (y-y_t)/d_c, (z-z_t)/d_c}; // Производные вычисленной дальности по координатам
                    A = dr_dx * dx_need;
                    AT = A.transpose();
                    Q += AT * A; // Заполнение матрицы нормальных уравнений
                    G += AT * (d_o - d_c); // Заполнение градиента
                    d_o_d_c.append(d_o - d_c);
                }
            }
        }
        NormalizeQG(NPAR, Q, G);
        dq = Q.inverse() * G; // Вычисление и добавление поправок
        UnNormalizeDq(NPAR, dq);

        ndq = sqrt(pow(dq(0), 2) + pow(dq(1), 2) + pow(dq(2), 2));

        cout << scientific << ndq << " " <<dq(6) << endl;

        for(int i1 = 0; i1 < 6; i1 ++){
            satellites[n_sat].XC0(i1) = satellites[n_sat].XC0(i1) + dq(i1);
        }
        if(NPAR > 6)for(int i1 = 6; i1 < NPAR; i1++){
            satellites[n_sat].LightCoeffs[i1-6] = satellites[n_sat].LightCoeffs[i1-6] + dq(i1);
        }
        if (ndq > acc) d_o_d_c.clear();
    }
    double mean_o_c;
    double std = standardDeviation2(d_o_d_c, mean_o_c);
    QVector<double> stats;
    stats.append(dq.norm());
    stats.append(mean_o_c);
    stats.append(std);
    stats.append(n_iters);
    if (NPAR > 6)stats.append(abs(dq(6)));
    return stats;
}

void UtochnenieSimple::Refine(){
    cout << "Start Refinig." << endl;
    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < NSAT; i ++){
        QVector<double> stats = RefineSat(i);
        cout << satellites[i].name << setprecision(4) << fixed
            << " | mean = " << 1e3*stats[1] << " m | std = " << 1e3*stats[2] << " m | n_iters = " << fixed << setprecision(0) << stats[3]
            << scientific << setprecision(3) << " | dq = " << stats[0];
            if (NPAR > 6) cout << scientific << " | dq_lc = " << stats[4];
            cout << endl;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    cout << fixed << setprecision(3) << "Finish refining. " << duration.count() << " second." << endl;
    cout << endl;
}

void UtochnenieSimple::MoveSat(int n_sat){
    QDateTime dt_p_n = dt_p.addSecs(SEANS);

    input_data_propagation_t input_gln1 = GetInputDataProp(dt_p, dt_p_n, n_sat, true);
    Propagation gln1 = GetPropagationObj();
    gln1.init(input_gln1);
    gln1.calculate();
    QVector<ephemeris_t> eph_r = gln1.getEphemeris();
    for(int i1 = 0; i1 < 6; i1++) satellites[n_sat].XE0(i1) = eph_r[eph_r.size()-1].pos[i1];

    input_data_propagation_t input_gln2 = GetInputDataProp(dt_p, dt_p_n, n_sat, false);
    Propagation gln2 = GetPropagationObj();
    gln2.init(input_gln2);
    gln2.calculate();
    QVector<ephemeris_t> eph_d = gln2.getEphemeris();
    for(int i1 = 0; i1 < 6; i1++) satellites[n_sat].XC0(i1) = eph_d[eph_d.size()-1].pos[i1];

    int ind;

    for(int K = NM-1; K >= NU; K--){
        ind = K - NU;
        for(int i1 = 0; i1 < 6; i1++){
            satellites[n_sat].EPH_E(K, i1) = satellites[n_sat].EPH_E(ind, i1);
            satellites[n_sat].EPH_C(K, i1) = satellites[n_sat].EPH_C(ind, i1);
        }
        satellites[n_sat].EPH_T_E(K) = satellites[n_sat].EPH_T_E(ind);
        satellites[n_sat].EPH_T_C(K) = satellites[n_sat].EPH_T_C(ind);
        for(int j = 0; j < NSAT; j++){
            for(int i1 = 0; i1 < 4; i1 ++) satellites[n_sat].OBS(K)(j, i1) = satellites[n_sat].OBS(ind)(j, i1);
            satellites[n_sat].UsageObs(K, j) = satellites[n_sat].UsageObs(ind, j);
        }
        for(int j = 0; j < NSTAN; j++){
            for(int i1 = 0; i1 < 4; i1 ++) satellites[n_sat].OBS_STAN(K)(j, i1) = satellites[n_sat].OBS_STAN(ind)(j, i1);
            satellites[n_sat].UsageObs_STAN(K, j) = satellites[n_sat].UsageObs_STAN(ind, j);
        }
    }
}

void UtochnenieSimple::MoveStan(int n_stan){
    int ind;

    for(int K = NM-1; K >= NU; K--){
        ind = K - NU;
        for(int i =0; i < 3; i++) stantions[n_stan].EPH_E(K, i) = stantions[n_stan].EPH_E(ind, i);
        stantions[n_stan].EPH_T_E(K) = stantions[n_stan].EPH_T_E(ind);
    }
}

void UtochnenieSimple::Moving(){
    cout << "Start moving." << endl;
    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < NSAT; i++){
        MoveSat(i);
    }
    for(int i = 0; i < NSTAN; i++){
        MoveStan(i);
    }
    dt_p = dt_p.addSecs(SEANS);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    cout << fixed << setprecision(3) << "Finish moving. " << duration.count() << " second." << endl;
    cout << endl;
}

int UtochnenieSimple::SatellitesVisible(Matrix<double, 3, 1> r_r, Matrix<double, 3, 1> r_t, int n_rec){
    const long double PI = std::acos(-1.0L); // PI

    double r_or = r_r.norm(); // Растояние от центра Земли до приемника
    double r_ot = r_t.norm(); // Растояние от центра Земли до передатчика
    Matrix<double, 3, 1> dr_tr = r_r - r_t;
    double r_tr = dr_tr.norm(); // Растояние между приемником и передатчиком

    double alpha = acos((pow(r_tr, 2) + pow(r_or, 2) - pow(r_ot, 2)) / (2 * r_tr * r_or)) * 180.0 / PI; // Угол от премника между центром Земли и передатчиком

    if (alpha > satellites[n_rec].GMAX || alpha < satellites[n_rec].GMIN) return 0; // Нет радиовидимости

    //double cos_beta = (pow(r_tr, 2) + pow(r_ot, 2) - pow(r_or, 2)) / (2 * r_tr * r_ot); // Косинус угла от передатчика между центром Земли и приемником
    //double R = r_ot * sqrt(1 - pow(cos_beta, 2)); // Длина перпендикуляра от центра Земли к прямой передатчик-приемник
    //double theta = 180 - alpha - acos(cos_beta) * 180 / PI; // Угол от центра Земли между передатчиком и приемником

    //if (R < r_shield && theta > 90) return false; // Нет визуальной видимости

    return 1; // Если все условия "невидиомости" не выполнились, значит методом исключений спутники друг друга видят
}

int UtochnenieSimple::StantionsVisible(Matrix<double, 3, 1> r_r, Matrix<double, 3, 1> r_t, int n_stan){
    const long double PI = std::acos(-1.0L); // PI

    double r_or = r_r.norm(); // Растояние от центра Земли до приемника
    double r_ot = r_t.norm(); // Растояние от центра Земли до передатчика
    Matrix<double, 3, 1> dr_tr = r_r - r_t;
    double r_tr = dr_tr.norm(); // Растояние между приемником и передатчиком

    long double alpha = acos((pow(r_ot, 2) + pow(r_tr, 2) - pow(r_or, 2)) / (2 * r_ot * r_tr)) * 180.0 / PI - 90;
    if (stantions[n_stan].GMIN > alpha || stantions[n_stan].GMAX < alpha) return 0;
    return 1;
}

void UtochnenieSimple::WriteFile(int n_sat){
    std::ofstream file_ref;
    file_ref.open("ResultsCoords/REF/"+satellites[n_sat].name+".txt", std::ios::app);
    std::ofstream file_def;
    file_def.open("ResultsCoords/DEF/"+satellites[n_sat].name+".txt", std::ios::app);

    for(int K = NU-1; K >= 0; K--){
        file_ref << fixed << setprecision(10) << satellites[n_sat].EPH_T_E(K) << "   ";
        for(int i1 = 0; i1 < 6; i1++) file_ref << fixed << setprecision(15) << satellites[n_sat].EPH_E(K, i1) << "   ";
        file_ref << endl;

        file_def << fixed << setprecision(10) << satellites[n_sat].EPH_T_C(K) << "   ";
        for(int i1 = 0; i1 < 6; i1++) file_def << fixed << setprecision(15) << satellites[n_sat].EPH_C(K, i1) << "   ";
        file_def << endl;
    }

    file_ref.close();
    file_def.close();

    if (NPAR > 6){
        std::ofstream file_lc;
        file_lc.open("ResultsCoords/LC/"+satellites[n_sat].name+".txt", std::ios::app);
        file_lc << fixed << satellites[n_sat].EPH_T_C(0) << "\t";
        for(int i1 = 0; i1 < 9; i1++){
            file_lc << setprecision(20) << satellites[n_sat].LightCoeffs(i1) - satellites[n_sat].LightCoeffs_ref(i1) << "\t";
        }file_lc << endl;
        file_lc.close();
    }
}

void UtochnenieSimple::WriteStanCoords(){
    for(int i = 0; i < NSTAN; i++){
        std::ofstream file_ref;
        file_ref.open("ResultsCoords/STAN/"+to_string(i)+".txt", std::ios::app);
        for(int K = 0; K < NM; K++){
            file_ref << stantions[i].EPH_E(K, 0) << " " << stantions[i].EPH_E(K, 1) << " " << stantions[i].EPH_E(K, 2) << endl;
        }
        file_ref.close();
    }
}

void UtochnenieSimple::ClearResultsFolder(){
    // Функция очистки папки с результатами перед новым запуском
    QString folderPath = "ResultsCoords/DEF";
    QDir dir_def(folderPath);
    if (!dir_def.exists()) return;
    QStringList filters1;
    filters1 << "*.txt";
    dir_def.setNameFilters(filters1);
    for (const QString& fileName : dir_def.entryList()) {
        dir_def.remove(fileName);
    }
    folderPath = "ResultsCoords/REF";
    QDir dir_ref(folderPath);
    if (!dir_ref.exists()) return;

    QStringList filters2;
    filters2 << "*.txt";
    dir_ref.setNameFilters(filters2);

    for (const QString& fileName : dir_ref.entryList()) {
        dir_ref.remove(fileName);
    }

    folderPath = "ResultsCoords/LC";
    QDir dir_lc(folderPath);
    if (!dir_lc.exists()) return;

    QStringList filters3;
    filters3 << "*.txt";
    dir_lc.setNameFilters(filters3);

    for (const QString& fileName : dir_lc.entryList()) {
        dir_lc.remove(fileName);
    }
}

void UtochnenieSimple::RunApp(){

    QVector<double> seanses_times;
    double seanses_mean_time;

    console_output = "START PROGRAMM\n";
    //emit WriteConsoleSignal();

    auto start_prog_time = std::chrono::high_resolution_clock::now();

    ClearResultsFolder();

    KF = NM;

    PropagateFirst();
    PropagateStantion();

    Observate();

    Refine();

    KF = NU;

    Moving();

    for(int ISES = 0; ISES < NSES; ISES ++){

        auto start_seans_time = std::chrono::high_resolution_clock::now();

        Propagate();
        PropagateStantion();

        Observate();

        for(int i = 0; i < NSAT; i++){WriteFile(i);}

        Refine();

        Moving();

        auto end_seans_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration_seans = end_seans_time - start_seans_time;
        seanses_times.append(duration_seans.count());
        if (seanses_times.size() > 10) seanses_times.removeFirst();
        seanses_mean_time = 0.0;
        for(int i = 0; i < seanses_times.size(); i++) seanses_mean_time += seanses_times[i] / seanses_times.size();

        cout << "\nFinished " << fixed << setprecision(2) << round((ISES+1)*10000/NSES)/100 << " % | Time left: " << seanses_mean_time * (NSES - ISES - 1) / 60.0 << " minutes." << endl;
    }

    auto end_prog_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_prog_time - start_prog_time;
    cout << "Programm finished work succesfully! " << fixed << setprecision(2) << duration.count() << " second." << endl;
}































