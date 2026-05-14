#include "utochnenie.h"


void Utochnenie::NormalizeQG(int n, Matrix<long double, Dynamic, Dynamic>& Q, Matrix<long double, Dynamic, 1>& G){
    // Нормировка нормальной матрицы и градиента
    Matrix<long double, 1, 15> scales = {sclx, sclx, sclx, sclv, sclv, sclv, scllc, scllc, scllc, scllc, scllc, scllc, scllc, scllc, scllc};
    for(int i = 0; i < n; i++){
        G(i) = G(i) * scales(i);
        for(int j = 0; j < n; j++){
            Q(i, j) = Q(i, j) * scales(i) * scales(j);
        }
    }
}

void Utochnenie::UnNormalizeDq(int n, Matrix<long double, Dynamic, 1>& dq, Matrix<long double, Dynamic, Dynamic>& Q, Matrix<long double, Dynamic, 1>& G){
    // Обратная нормировка нормальной матрицы, градиента и вектора поправок в параметры движения
    Matrix<long double, 1, 15> scales = {sclx, sclx, sclx, sclv, sclv, sclv, scllc, scllc, scllc, scllc, scllc, scllc, scllc, scllc, scllc};
    for(int i = 0; i < n; i++){
        dq(i) = dq(i) * scales(i);
    }
    for(int i = 0; i < n; i++){
        G(i) = G(i) / scales(i);
        for(int j = 0; j < n; j++){
            Q(i, j) = Q(i, j) / ( scales(i) * scales(j));
        }
    }
}

void Utochnenie::RecountDempfSat(int n_sat){
    int n = satellites[n_sat].mists.size();
    if(n < 4) {WriteSatDempf(n_sat); return;}

    long double xy = 0.0L, x2 = 0.0L, sx = 0.0L, sy = 0.0L;
    for(int i = 0; i < n; i++){
        xy += i * satellites[n_sat].mists[i];
        sy += satellites[n_sat].mists[i];
        x2 += i * i;
        sx += i;
    }

    long double k = (n * xy - sx * sy) / (n * x2 - pow(sx, 2));

    long double K = 1 + 0.05 / (1 + exp(-k)) - 0.025;

    satellites[n_sat].dempf /= K;

    WriteSatDempf(n_sat);
}

Utochnenie::Utochnenie()
{
    // Параметры нормировки
    sclx = 1e5;
    sclv = 1e0;
    scllc = 1e1;

    //Флаг завершения работы расчетов
    fl_stop = false;

    // Максимальное число итераций ------------------------- вынести на главное окно
    max_iters = 2;
}

void Utochnenie::SetNSAT(int n){
    NSAT = n;

    satelliteGenerators.resize(NSAT);

    for (int i = 0; i < NSAT; ++i) {
        satelliteGenerators[i].seed(i);
        satellite sat;
        sat.IdentedSysErr.resize(NSAT, 1);
        sat.IdentedSysErr.setZero();
        satellites.push_back(sat);
    }
}

void Utochnenie::SetNSTAN(int n){
    NSTAN = n;
    for(int i = 0; i < NSTAN; i++){
        stantion ns;
        stantions.push_back(ns);
    }
}

void Utochnenie::SetSatCoords(Matrix<long double, Dynamic, 6> coords){
    for(int i = 0; i < NSAT; i++){
        for(int j = 0; j < 6; j++){
            satellites[i].XE0(j) = coords(i, j);
            satellites[i].XC0(j) = coords(i, j);
        }
    }
}

void Utochnenie::SetStanCoords(Matrix<long double, Dynamic, 3> coords){
    for(int i = 0; i < NSTAN; i++){
        stantions[i].X = coords(i, 0);
        stantions[i].Y = coords(i, 1);
        stantions[i].Z = coords(i, 2);
    }
}

void Utochnenie::SetSatNames(Matrix<QString, Dynamic, 1> names){
    for(int i = 0; i < NSAT; i++){
        satellites[i].name = names(i).toStdString();
    }
}

void Utochnenie::SetLPC(Matrix<long double, Dynamic, 9> lpc){
    for(int i = 0; i < NSAT; i ++){
        for(int j = 0; j < 9; j++) {
            satellites[i].LightCoeffs(j) = lpc(i, j);
            satellites[i].LightCoeffs_ref(j) = lpc(i, j);
        }
    }
}

void Utochnenie::SetSatAngle(Matrix<long double, Dynamic, 2> angs){
    for(int i = 0; i < NSAT; i++){
        satellites[i].GMIN = angs(i, 0);
        satellites[i].GMAX = angs(i, 1);
    }
}

void Utochnenie::SetStanAngle(Matrix<long double, Dynamic, 2> angs){
    for(int i = 0; i < NSTAN; i++){
        stantions[i].GMIN = angs(i, 0);
        stantions[i].GMAX = angs(i, 1);
    }
}

void Utochnenie::SetCyclogramSatSat(Matrix<int, Dynamic, Dynamic> cycl){
    for(int i = 0; i < NSAT; i++){
        satellites[i].Cyclogram.resize(NSAT);
        for(int j = 0; j < NSAT; j++){
            satellites[i].Cyclogram(j) = cycl(i, j);
        }
    }
}

void Utochnenie::SetCyclogramSatStan(Matrix<int, Dynamic, Dynamic> cycl){
    for(int i = 0; i < NSAT; i++){
        satellites[i].CyclogramStan.resize(NSTAN);
        for(int j = 0; j < NSTAN; j++){
            satellites[i].CyclogramStan(j) = cycl(i, j);
        }
    }
}

void Utochnenie::SetNoises(long double sat, long double stan, long double sys){
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
            for(int j = i; j < NSAT; j++){
                if(i != j){
                    SysErrSat(i, j) = dist(gen);
                    SysErrSat(j, i) = SysErrSat(i, j);
                }
            }
            for(int j = 0; j < NSTAN; j++){
                SysErrStan(i, j) = dist(gen);
            }
        }
    }
}

void Utochnenie::SetDempf(long double d){
    dempf = d;
    for(int i = 0 ; i < NSAT; i ++){
        satellites[i].dempf = d;
    }
}

void Utochnenie::SetAcc(long double a){
    acc = a;
}

string Utochnenie::cleanLine(string line) {
    size_t pos = line.find(" - ");
    if (pos != std::string::npos) {
        return line.substr(0, pos);
    }
    return line;
}

long double Utochnenie::QDate2Julian(QDateTime dt){
    long long jd_integer = dt.date().toJulianDay();
    double dayFraction = (dt.time().hour() * 3600.0 +
                          dt.time().minute() * 60.0 +
                          dt.time().second()) / 86400.0;
    long double fullJD = jd_integer - 0.5 + dayFraction;
    return fullJD;
}

long double Utochnenie::Sid2000(long double djd){
    const long double PI2 = 6.283185307179586476925286766559L;
    const long double S2R = 7.272205216643039903848711535369E-5L;

    long double ut1 = djd - 2400000.5L;
    long double tu = (ut1 - 51544.5L) / 36525.0L;

    long double dayFraction = (ut1 - std::floor(ut1)) * PI2;

    long double secular = (24110.54841L + (8640184.812866L +
                          (0.093104L - 6.2e-6L * tu) * tu) * tu) * S2R;

    long double s = std::fmod(dayFraction + secular, PI2);

    if (s < 0) s += PI2;
    return s;
}

void Utochnenie::SetTimeNet(long double dt, long double block, long double gap, long double interval){
    DT = dt;
    BLOCK = block;
    GAP = gap;
    INTERVAL = interval;
    SEANS = block + gap;

    NM = INTERVAL / DT;

    for(int i1 = 0; i1 < NSAT; i1++){
        satellites[i1].Derivatives.resize(NM, 45);
        satellites[i1].DerivativesY.resize(NM, 45);
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

void Utochnenie::SetNavigationFrames(QDateTime dt_s_, QDateTime dt_f_){
    dt_s = dt_s_;
    dt_f = dt_f_;
    dt_p = dt_s_;

    NSES = (dt_s.secsTo(dt_f) + INTERVAL) / INTERVAL;
    cout << NSES << endl;
}

void Utochnenie::SetPerturb(int geo_nm, int geo_mn, bool sun, bool moon, bool planets, bool rad, bool rel_eff, bool tides){
    GeoNM = geo_nm;
    GeoMN = geo_mn;
    pertMoon = moon;
    pertSun = sun;
    pertPlanets = planets;
    pertTides = tides;
    pertRadiation = rad;
    pertRelatEffects = rel_eff;
}

void Utochnenie::SetNParamsRefine(int n){
    NPAR = n;
    for(int i = 0; i < NSAT; i ++){
        satellites[i].Q.resize(n, n);
        satellites[i].Q.setZero();
        satellites[i].G.resize(n, 1);
        satellites[i].G.setZero();
    }
}

input_data_propagation_t Utochnenie::GetInputDataProp(QDateTime dt_s_, QDateTime dt_f_, int n_sat, bool fl_ref){
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

    input_gln.integrOrder = 10;
    input_gln.integrStep = 1000.0;
    input_gln.integrEtol = 1e-8;

    QVector<long double> lightCoef(9);
    if(fl_ref)for (int i = 0; i < 9; i ++) lightCoef[i] = satellites[n_sat].LightCoeffs_ref(i);
    else for (int i = 0; i < 9; i ++) lightCoef[i] = satellites[n_sat].LightCoeffs(i);
    input_gln.lightCoef = lightCoef;

    x.clear(); y.clear(); lightCoef.clear();

    return input_gln;
}

Propagation Utochnenie::GetPropagationObj(){
    Propagation gln;
    gln.setFileEop("nm/eop/EOP.DAT");
    gln.setFileFond("nm/fond/fond438.bin");
    return gln;
}

void Utochnenie::PropagateRef(int n_sat){
    long double sec_btw_par_and_start = dt_s.secsTo(dt_p);
    QDateTime start_prop = dt_p.addSecs(-DT * (NM-1));
    QDateTime dt_p_ = dt_p;
    input_data_propagation_t input_gln = GetInputDataProp(start_prop, dt_p_, n_sat, true);
    Propagation gln = GetPropagationObj();
    gln.init(input_gln);
    gln.calculate();
    QVector<ephemeris_t> eph = gln.getEphemeris();
    for(int K = 0; K < NM; K++){
        for(int i1 = 0; i1 < 6; i1 ++) satellites[n_sat].EPH_E(K, i1) = eph[K].pos[i1];
        satellites[n_sat].EPH_T_E(K) = sec_btw_par_and_start + eph[K].time;
    }
    eph.clear();
}

void Utochnenie::PropagateFirst(){
    int n_sat = 0;

    sig_setLocalProgress(0, "Прогноз ", "");

    while(n_sat < NSAT && !fl_stop){
        long double sec_btw_par_and_start = dt_s.secsTo(dt_p);
        QDateTime start_prop = dt_p.addSecs(-DT * (NM - 1));
        QDateTime dt_p_ = dt_p;
        input_data_propagation_t input_gln = GetInputDataProp(start_prop, dt_p_, n_sat, true);
        Propagation gln = GetPropagationObj();
        gln.init(input_gln);
        gln.calculate();

        sig_setLocalProgress(100*(n_sat+1)/NSAT, "Прогноз ", QString::fromStdString(satellites[n_sat].name));

        QVector<ephemeris_t> eph = gln.getEphemeris();
        for(int K = 0; K < NM; K++){
            for(int i1 = 0; i1 < 6; i1 ++) satellites[n_sat].EPH_E(K, i1) = eph[K].pos[i1];
            satellites[n_sat].EPH_T_E(K) = sec_btw_par_and_start + eph[K].time;
            for(int i1 = 0; i1 < 6; i1 ++) satellites[n_sat].EPH_C(K, i1) = eph[K].pos[i1];
            satellites[n_sat].EPH_T_C(K) = sec_btw_par_and_start + eph[K].time;
            for(int i1 = 0; i1 < 45; i1++) satellites[n_sat].Derivatives(K, i1) = eph[K].derivatives_x[i1];
            for(int i1 = 0; i1 < 45; i1++) satellites[n_sat].DerivativesY(K, i1) = eph[K].derivatives_y[i1];
            eph.clear();
        }
        n_sat++;
    }

    sig_setLocalProgress(100, "Прогноз ", "");
}

void Utochnenie::PropagateDef(int n_sat){
    long double sec_btw_par_and_start = dt_s.secsTo(dt_p);
    QDateTime start_prop = dt_p.addSecs(-DT * (NM - 1));
    QDateTime dt_p_ = dt_p;
    input_data_propagation_t input_gln = GetInputDataProp(start_prop, dt_p_, n_sat, false);
    Propagation gln = GetPropagationObj();
    gln.init(input_gln);
    gln.calculate();
    QVector<ephemeris_t> eph = gln.getEphemeris();
    for(int K = 0; K < NM; K++){
        for(int i1 = 0; i1 < 6; i1 ++) satellites[n_sat].EPH_C(K, i1) = eph[K].pos[i1];
        satellites[n_sat].EPH_T_C(K) = sec_btw_par_and_start + eph[K].time;
        for(int i1 = 0; i1 < 45; i1++) satellites[n_sat].Derivatives(K, i1) = eph[K].derivatives_x[i1];
        for(int i1 = 0; i1 < 45; i1++) satellites[n_sat].DerivativesY(K, i1) = eph[K].derivatives_y[i1];
    }
    eph.clear();
}

void Utochnenie::PropagateStantion(){
    Matrix<long double, 3, 3> A;
    Matrix<long double, 3, 1> rv, rv_jd;
    long double sec_btw_par_and_start = dt_s.secsTo(dt_p);
    A.setZero();
    A(2, 2) = 1;
    long double jd, sid2000;
    QDateTime dt_mom;
    long double cos_s, sin_s;
    for(int K = 0; K < NM; K++){
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

void Utochnenie::Propagate(){
    sig_setLocalProgress(0, "Прогноз ", "");
    int i = 0;
    while(i < NSAT && !fl_stop){
        PropagateDef(i);
        PropagateRef(i);
        sig_setLocalProgress(100*(i+1)/NSAT, "Прогноз ", QString::fromStdString(satellites[i].name));
        i++;
    }
    sig_setLocalProgress(100, "Прогноз ", "");
}

void Utochnenie::ObservateSat(int n_sat){
    long double mean = 0.0;
    normal_distribution<long double> dist(mean, NoiseSat);
    normal_distribution<long double> dist_stan(mean, NoiseStan);
    long double stoch_noise, sys_noise;
    Matrix<double, 3, 1> r_r, r_t;
    long double x_r, x_t, x_t_, y_r, y_t, y_t_, z_r, z_t, z_t_, d_o;
    for(int j = 0; j < NSAT; j++){
        if (n_sat != j){
            for(int K = 0; K < NM; K++){
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
        for(int K = 0; K < NM; K++){
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

void Utochnenie::Observate(){
    int i = 0;
    sig_setLocalProgress(0, "Формирвоание наблюдений ", "");
    while(i < NSAT && !fl_stop){
        ObservateSat(i);
        sig_setLocalProgress(100*(i+1)/NSAT, "Формирование наблюдений ", QString::fromStdString(satellites[i].name));
        i++;
    }
    sig_setLocalProgress(100, "Формирование наблюдений ", "");
}

void Utochnenie::IdentSysErr(int n_sat){
    long double d_o, d_c, x1, y1, z1, x2, y2, z2;
    long double discrepancies;
    int n_obs;
    for(int j = 0; j < NSAT; j++){
        discrepancies = 0.0L;
        n_obs = 0;
        if (n_sat != j){
            for(int K = 0; K < NM; K++){
                d_o = satellites[n_sat].OBS(K)(j, 3);
                x1 = satellites[n_sat].OBS(K)(j, 0);
                y1 = satellites[n_sat].OBS(K)(j, 1);
                z1 = satellites[n_sat].OBS(K)(j, 2);
                x2 = satellites[n_sat].EPH_C(K, 0);
                y2 = satellites[n_sat].EPH_C(K, 1);
                z2 = satellites[n_sat].EPH_C(K, 2);
                d_c = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2));
                discrepancies += (d_o - d_c);
                n_obs ++;
            }
        }

        satellites[n_sat].IdentedSysErr(j) = discrepancies / n_obs;
    }
}

bool Utochnenie::ObsInBlock(int K, int n_sat){
    long double T = satellites[n_sat].EPH_T_E(K);

    if (T == 0) return 1;

    if (T < 0) T += INTERVAL;

    long double fmodt = fmod(T, SEANS);

    if (fmodt <= BLOCK || abs(fmodt-BLOCK) < 1e-5) return 1;

    return 0;
}

void Utochnenie::RefineSat(int n_sat){
    Matrix<long double, Dynamic, Dynamic> Q, dQ;
    Q.resize(NPAR, NPAR);
    dQ.resize(NPAR, NPAR);

    Matrix<long double, Dynamic, 1> G, dq, dG;
    dq.resize(NPAR, 1);
    G.resize(NPAR, 1);
    dG.resize(NPAR, 1);

    Matrix<long double, 1, 3> dr_dx;
    long double x, y, z;
    long double d_c;
    long double d_o, x_t, y_t, z_t;
    Matrix<long double, 3, Dynamic> dx_need;
    dx_need.resize(3, NPAR);
    Matrix<long double, 1, Dynamic> A;
    A.resize(1, NPAR);
    Matrix<long double, Dynamic, 1> AT;
    AT.resize(NPAR, 1);
    int n_iters = 0;
    QVector<double> d_o_d_c;
    long double ndq = 1;
    int n_obs;
    double mean_o_c, std;

    IdentSysErr(n_sat);

    while(acc < ndq && n_iters < max_iters){

        PropagateDef(n_sat);

        n_iters += 1;
        dQ.setZero();
        dG.setZero();
        n_obs = 0;

        for(int K = 0; K < NM; K++){
            x = satellites[n_sat].EPH_C(K, 0);
            y = satellites[n_sat].EPH_C(K, 1);
            z = satellites[n_sat].EPH_C(K, 2);
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
                    d_o = satellites[n_sat].OBS(K)(j, 3) - satellites[n_sat].IdentedSysErr(j);
                    d_c = sqrt(pow(x-x_t, 2) + pow(y-y_t, 2) + pow(z-z_t, 2));
                    dr_dx = {(x-x_t)/d_c, (y-y_t)/d_c, (z-z_t)/d_c}; // Производные вычисленной дальности по координатам
                    A = dr_dx * dx_need;
                    AT = A.transpose();
                    dQ += AT * A; // Заполнение матрицы нормальных уравнений
                    dG += AT * (d_o - d_c); // Заполнение градиента
                    d_o_d_c.append(d_o - d_c);
                    n_obs++;
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
                    dQ += AT * A; // Заполнение матрицы нормальных уравнений
                    dG += AT * (d_o - d_c); // Заполнение градиента
                    d_o_d_c.append(d_o - d_c);
                    n_obs++;
                }
            }
        }

        for(int i = 0; i < NPAR; i++){
            for(int j = 0; j < NPAR; j++){
                Q(i, j) = satellites[n_sat].Q(i, j) * pow(satellites[n_sat].dempf, 2) + dQ(i, j);
            }
            G(i) = satellites[n_sat].G(i) * satellites[n_sat].dempf + dG(i);
        }

        NormalizeQG(NPAR, Q, G);
        dq = Q.inverse() * G; // Вычисление и добавление поправок
        UnNormalizeDq(NPAR, dq, Q, G);

        ndq = sqrt(pow(dq(0), 2) + pow(dq(1), 2) + pow(dq(2), 2));

        for(int i1 = 0; i1 < 6; i1 ++){
            satellites[n_sat].XC0(i1) = satellites[n_sat].XC0(i1) + dq(i1);
        }
        if(NPAR == 15)for(int i1 = 6; i1 < NPAR; i1++){
            satellites[n_sat].LightCoeffs[i1-6] = satellites[n_sat].LightCoeffs[i1-6] + dq(i1);
        }
        if (ndq > acc && n_iters < max_iters) {
            d_o_d_c.clear();
        }
    }

    for(int i = 0; i < NPAR; i++){
        for(int j = 0; j < NPAR; j++){
            satellites[n_sat].Q(i, j) = Q(i, j);
        }
        satellites[n_sat].G(i) = G(i);
    }

    sig_setNIters(n_sat, n_iters);

    std = standardDeviation2(d_o_d_c, mean_o_c);
    d_o_d_c.clear();
    long double condition_number = getConditionNumberSVD(Q);

    sig_setMean(mean_o_c*100000.0, n_sat);
    sig_setSTD(std*100000.0, n_sat);
    sig_setCond(condition_number, n_sat);
    sig_setDqX(ndq, n_sat);
    sig_setNObs(n_obs, n_sat);
    if(NPAR == 15) sig_setDqSD(dq(6), n_sat);
    else sig_setDqSD(0, n_sat);
}

void Utochnenie::Refine(){
    int i = 0;
    sig_setLocalProgress(0, "Уточнение ", QString::fromStdString(satellites[i].name));
    while(i < NSAT && !fl_stop){
        RefineSat(i);
        sig_setLocalProgress(100*(i+1)/NSAT, "Уточнение ", QString::fromStdString(satellites[i].name));
        i++;
    }
    sig_setLocalProgress(100, "Уточнение ", "");

}

void Utochnenie::MoveSat(int n_sat){
    QDateTime dt_p_n = dt_p.addSecs(INTERVAL);

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

    Matrix<long double, Dynamic, Dynamic> DX, Q1, Q2;
    DX.resize(NPAR, NPAR);
    Q1.resize(NPAR, NPAR);
    Q2.resize(NPAR, NPAR);
    Q2.setZero();
    Q1.setZero();
    Matrix<long double, Dynamic, 1> G1, G2;
    G1.resize(NPAR, 1);
    G2.resize(NPAR, 1);
    G1.setZero();
    G2.setZero();

    DX.setZero();

    for(int i = 0; i < 3; i ++){
        for(int j = 0; j < NPAR; j++){
            DX(i, j) = eph_d[eph_d.size()-1].derivatives_x[(i-0) * 15 + j];
        }
    }
    for(int i = 3; i < 6; i ++){
        for(int j = 0; j < NPAR; j++){
            DX(i, j) = eph_d[eph_d.size()-1].derivatives_y[(i-3) * 15 + j];
        }
    }
    if (NPAR == 15){
        for(int i = 6; i < 15; i++){
            DX(i, i) = 1.0L;
        }
    }

    for(int i = 0; i < NPAR; i ++){
        for(int j = 0; j < NPAR; j++){
            Q1(i, j) = satellites[n_sat].Q(i, j);
        }
        G1(i) = satellites[n_sat].G(i);
    }

    Matrix<long double, Dynamic, Dynamic> DX_inv;
    DX_inv.resize(NPAR, NPAR);
    DX_inv = DX.inverse();

    Q2 = DX_inv.transpose() * Q1 * DX_inv;
    G2 = DX_inv.transpose() * G1;

    for(int i = 0; i < NPAR; i ++){
        for(int j = 0; j < NPAR; j ++){
            satellites[n_sat].Q(i, j) = Q2(i, j);
        }
        satellites[n_sat].G(i) = G2(i);
    }

    eph_r.clear();
    eph_d.clear();
}

void Utochnenie::Moving(){
    int i = 0;
    sig_setLocalProgress(0, "Перенос ", "");
    while(i < NSAT && !fl_stop){
        MoveSat(i);
        sig_setLocalProgress(100*(i+1)/NSAT, "Перенос ", QString::fromStdString(satellites[i].name));
        i++;
    }
    dt_p = dt_p.addSecs(INTERVAL);
    sig_setLocalProgress(100, "Перенос ", "");
}

int Utochnenie::SatellitesVisible(Matrix<double, 3, 1> r_r, Matrix<double, 3, 1> r_t, int n_rec){
    const long double PI = std::acos(-1.0L); // PI

    double r_or = r_r.norm(); // Растояние от центра Земли до приемника
    double r_ot = r_t.norm(); // Растояние от центра Земли до передатчика
    Matrix<double, 3, 1> dr_tr = r_r - r_t;
    double r_tr = dr_tr.norm(); // Растояние между приемником и передатчиком

    double alpha = acos((pow(r_tr, 2) + pow(r_or, 2) - pow(r_ot, 2)) / (2 * r_tr * r_or)) * 180.0 / PI; // Угол от премника между центром Земли и передатчиком

    if (alpha > satellites[n_rec].GMAX || alpha < satellites[n_rec].GMIN) return 0; // Нет радиовидимости

    return 1; // Если все условия "невидиомости" не выполнились, значит методом исключений спутники друг друга видят
}

int Utochnenie::StantionsVisible(Matrix<double, 3, 1> r_r, Matrix<double, 3, 1> r_t, int n_stan){
    const long double PI = std::acos(-1.0L); // PI

    double r_or = r_r.norm(); // Растояние от центра Земли до приемника
    double r_ot = r_t.norm(); // Растояние от центра Земли до передатчика
    Matrix<double, 3, 1> dr_tr = r_r - r_t;
    double r_tr = dr_tr.norm(); // Растояние между приемником и передатчиком

    long double alpha = acos((pow(r_ot, 2) + pow(r_tr, 2) - pow(r_or, 2)) / (2 * r_ot * r_tr)) * 180.0 / PI - 90;
    if (stantions[n_stan].GMIN > alpha || stantions[n_stan].GMAX < alpha) return 0;
    return 1;
}

void Utochnenie::WriteSTWErrs(int n_sat){
    std::ofstream file_;
    file_.open("ResultsCoords/"+satellites[n_sat].name+".txt", std::ios::app);
    for(int K = NM-1; K >= 0; K--){
        Vector6ld ref, def;
        ref << satellites[n_sat].EPH_E(K, 0), satellites[n_sat].EPH_E(K, 1), satellites[n_sat].EPH_E(K, 2),
                satellites[n_sat].EPH_E(K, 3), satellites[n_sat].EPH_E(K, 4), satellites[n_sat].EPH_E(K, 5);
        def << satellites[n_sat].EPH_C(K, 0), satellites[n_sat].EPH_C(K, 1), satellites[n_sat].EPH_C(K, 2),
                satellites[n_sat].EPH_C(K, 3), satellites[n_sat].EPH_C(K, 4), satellites[n_sat].EPH_C(K, 5);
        Vector3ld stw = error_to_stw(ref, def);
        long double r3 = sqrt(pow(stw[0], 2) + pow(stw[1], 2) + pow(stw[2], 2));
        file_ << fixed << setprecision(10) << satellites[n_sat].EPH_T_E(K) / 86400.0L << "   ";
        for(int i1 = 0; i1 < 3; i1++) file_ << fixed << setprecision(15) << stw[i1] * 1e5 << "   ";
        file_ << r3*1e5 << endl;
    }
    file_.close();
}

void Utochnenie::WriteSatellitesOBS(int n_sat){
    std::ofstream file_;
    file_.open("OBS/"+satellites[n_sat].name+".txt", std::ios::app);
    for(int K = 0; K < NM; K++){
        for(int j = 0; j < NSAT; j++){
            file_ << fixed << setprecision(10) << satellites[n_sat].EPH_T_E(K) << "   ";
            file_ << fixed << setprecision(15);
            file_ << satellites[n_sat].OBS(K)(j, 3) << "   " << satellites[n_sat].UsageObs(K, j) << endl;
        }
    }
    file_.close();
}

void Utochnenie::WriteStartParams(){
    std::ofstream file_;
    file_.open("ResultsCoords/StartParams.txt");
    int Ys, Ms, Ds, hs, ms, ss, Ye, Me, De, he, me, se;
    Ys = dt_s.date().year();
    Ms = dt_s.date().month();
    Ds = dt_s.date().day();
    hs = dt_s.time().hour();
    ms = dt_s.time().minute();
    ss = dt_s.time().second();
    Ye = dt_f.date().year();
    Me = dt_f.date().month();
    De = dt_f.date().day();
    he = dt_f.time().hour();
    me = dt_f.time().minute();
    se = dt_f.time().second();
    file_ << "Начало: " << Ys << "." << Ms << "." << Ds << " " << hs << ":" << ms << ":" << ss << endl;
    file_ << "Конец: " << Ye << "." << Me << "." << De << " " << he << ":" << me << ":" << se << endl;
    for(int i = 0; i < NSAT; i ++) file_ << satellites[i].name << " ";
    file_ << endl;
    file_ << "Время между обновлениями: " << INTERVAL << " c." << endl;
    file_ << "Время между наблюдениями: " << DT << " c." << endl;
    file_ << "Сеанс наблюдений: " << SEANS << " c." << endl;
    file_ << "Разрыв между сеансами наблюдений: " << GAP << " c." << endl;
    file_ << "Возмущения: " << endl;
    file_ << "*\tВозмущения геопотенциала " << GeoNM << " " << GeoMN << endl;
    file_ << "*\tСолнце " << pertSun << endl;
    file_ << "*\tЛуна " << pertMoon << endl;
    file_ << "*\tПланеты " << pertPlanets << endl;
    file_ << "*\tСветовое давление " << pertRadiation << endl;
    file_ << "*\tРелятивистика " << pertRelatEffects << endl;
    file_ << "*\tПриливы " << pertTides << endl;
    file_ << "Демпфирующий множитель: " << dempf << endl;
    file_ << "Максимальное число итераций: " << max_iters << endl;
    file_ << "Критерий выхода: " << acc << endl;
    file_ << "Уточняемые параметры: " << NPAR << endl;
    file_ << "Стохасические шумы МСИ: " << NoiseSat << " см." << endl;
    file_ << "Стохасические шумы наблюдений НС: " << NoiseStan << " см." << endl;
    file_ << "Систематические шумы: " << SysErr << " см." << endl;
    file_.close();
}

void Utochnenie::WriteSatDempf(int n_sat){
    std::ofstream file_;
    file_.open("ResultsCoords/Dempf" + satellites[n_sat].name + ".txt", ios::app);
    file_ << dt_s.secsTo(dt_p) << " " << satellites[n_sat].dempf << endl;
    file_.close();
}

void Utochnenie::ClearResultsFolder(){
    // Функция очистки папки с результатами перед новым запуском
    QString folderPath = "ResultsCoords/";
    QDir dir_def(folderPath);
    if (!dir_def.exists()) return;
    QStringList filters1;
    filters1 << "*.txt";
    dir_def.setNameFilters(filters1);
    for (const QString& fileName : dir_def.entryList()) {
        dir_def.remove(fileName);
    }
    for(int i = 0; i < NSAT; i++){
        std::ofstream file_;
        file_.open("ResultsCoords/"+satellites[i].name+".txt", std::ios::app);
        file_ << "     t_days                S_cm                T_cm                W_cm                R_cm" << endl;
        file_.close();
    }
}

void Utochnenie::RunApp(){

    QVector<double> seanses_times;
    double seanses_mean_time;
    int percents;
    QString pbText;

    ClearResultsFolder();
    WriteStartParams();

    if(!fl_stop){
        PropagateFirst();
    }else{
        return;
    }

    if(!fl_stop){
        PropagateStantion();
    }else{
        return;
    }

    if(!fl_stop){
        Observate();
    }else{
        return;
    }

    if(!fl_stop){
        Refine();
    }else{
        return;
    }

    if(!fl_stop){
        Moving();
    }else{
        return;
    }

    sig_clearNIters();
    sig_clearMean();
    sig_clearSTD();
    sig_clearCond();
    sig_clearDqX();
    sig_clearDqSD();
    sig_clearNObs();

    for(int ISES = 0; ISES < NSES; ISES ++){

        auto start_seans_time = std::chrono::high_resolution_clock::now();

        if(!fl_stop){
            Propagate();
        }else{
            return;
        }

        if(!fl_stop){
            PropagateStantion();
        }else{
            return;
        }

        if(!fl_stop){
            Observate();
        }else{
            return;
        }

        for(int i = 0; i < NSAT; i++){
            if(!fl_stop){
                WriteSTWErrs(i);
            }else{
                return;
            }
        }

        if(!fl_stop){
            Refine();
        }else{
            return;
        }

        if(!fl_stop){
            Moving();
        }else{
            return;
        }

        sig_clearNIters();
        sig_clearMean();
        sig_clearSTD();
        sig_clearCond();
        sig_clearDqX();
        sig_clearDqSD();
        sig_clearNObs();

        auto end_seans_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration_seans = end_seans_time - start_seans_time;
        seanses_times.append(duration_seans.count());
        if (seanses_times.size() > 10) seanses_times.removeFirst();
        seanses_mean_time = 0.0;
        for(int i = 0; i < seanses_times.size(); i++) seanses_mean_time += seanses_times[i] / seanses_times.size();

        percents = round((ISES+1)*10000/NSES)/100;

        pbText = "Оставшееся время " + QString::number(seanses_mean_time * (NSES - ISES - 1) / 60.0, 'f', 2) + " минут. ";
        pbText += QString::number(dt_s.secsTo(dt_p) / 86400.0, 'f', 1) + " / " + QString::number(dt_s.secsTo(dt_f) / 86400.0 + 1, 'f', 1) + " дней.";
        sig_setProgress(percents, pbText);
    }
}

void Utochnenie::stop(){
    fl_stop = true;
}






























