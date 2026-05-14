#include "perturbations.h"
#include <QDebug>
#include <QtMath>



Perturbations::Perturbations() {

}

void Perturbations::init(){

}

void Perturbations::forse(int nxy, QVector<long double> x, QVector<long double> y, long double t, QVector<long double>& f){
    //функция правых частей
    //учет влияние только центрального тела
    QVector<long double> xMoon(6),xSun(6);
    QVector<QVector<long double>> xPlanets(9,QVector<long double>(3));
    QVector<QVector<long double>> f2(3,QVector<long double>(3)); // Массив производных от сил по координатам
    QVector<QVector<long double>> DFSL(21,QVector<long double>(3)); // Массив производных от РД(21) по координатам

    const long double DLT[3][3] = {
        {1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}
    };

    //расстояние до НКА
    long double R2 = x[0]*x[0] + x[1]*x[1] + x[2]*x[2];
    long double rx = sqrt(R2);
    long double R3 = R2*rx;

    //центральное тело (уравнения второго порядка)
    if (nxy == 48){
        f[0] = -_GE * x[0] / (R3);
        f[1] = -_GE * x[1] / (R3);
        f[2] = -_GE * x[2] / (R3);

        //вторые производные центральной силы
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                f2[i][j] = - _GE * (DLT[i][j] - 3.0 * x[i] * x[j] / R2) / R3;
            }
        }
    }

    //чтение фонда
    if (_isSun || _isMoon || _isPlanets || _isTides || _isRelativistic || _isRadiationPressure){
        read438(t, xMoon, xSun, xPlanets);

    }

    if (_isEarthHarmonics){
       if (_isTides){
           changeGarmCoef(t);           
       }
       geopot(t, x, f, f2);
    }


    if (_isTides){
        tides(x,xMoon,xSun,f);
    }

    if (_isMoon){
        //gravThirdBody(_GM,x,xMoon,true, f,f2);
        gravThirdBody(_GM,x,xMoon,false, f,f2);

    }

    if (_isSun){
        gravThirdBody(_GS,x,xSun,false,f,f2);
    }

    if (_isPlanets) {
        //ускорения от каждой из 9 планет кроме Земли
        for (int i=0; i<9; i++){
            if (i==2) continue;
             gravThirdBody(_GPLS[i],x,xPlanets[i],false, f,f2);
        }
    }

    if (_isRelativistic){
        rltvt(x,y,xSun,f);
    }
    if (_isRadiationPressure){
        radPressure2(x,y,xSun,xMoon, f,DFSL, f2);
    }

    //для частных производных
    if (nxy>3) {
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 15; j++){
                int index = i * 15 + j + 3;
                f[index] = f2[i][0] * x[3+j] + f2[i][1] * x[18+j] + f2[i][2] * x[33+j];
            }
        }
        if (_isRadiationPressure){
            for (int i = 0; i < 9; i++) {
                f[i+9] += DFSL[i][0];
                f[i+24] += DFSL[i][1];
                f[i+39] += DFSL[i][2];
            }
        }
    }

    //Очищение
    xMoon.clear(); xSun.clear(); xPlanets.clear(); DFSL.clear(); f2.clear();

}

void Perturbations::gravThirdBody(long double mu, QVector<long double> x, QVector<long double> xM, bool pert, QVector<long double> &f,QVector<QVector<long double>>& f2)
{
     //расстояние от Центра Земли до объекта
    long double R2 = xM[0]*xM[0] + xM[1]*xM[1] + xM[2]*xM[2];
    long double rM = sqrt(R2);
    long double R3 = rM*rM*rM;
    //расстояние от ИСЗ до объекта
    long double D2 = (xM[0]-x[0])*(xM[0]-x[0]) + (xM[1]-x[1])*(xM[1]-x[1]) + (xM[2]-x[2])*(xM[2]-x[2]);
    long double dM = sqrt(D2);
    long double D3 = dM*dM*dM;

    //ускорения от грав. возмущений объекта
    for (int i=0; i<3; i++ ){
        f[i] +=  mu*( (xM[i]-x[i]) / D3  -  xM[i] / R3);
    }

    //возмущение от влияние 2ой зональной гармоники на движение объекта(Луны)
    if (pert) {
        f[0] += -1.5*mu*0.001082626*pow(_RE,2)/pow(rM,5)*(1.0 - 5.*pow(xM[2]/rM,2)) * xM[0];
        f[1] += -1.5*mu*0.001082626*pow(_RE,2)/pow(rM,5)*(1.0 - 5.*pow(xM[2]/rM,2)) * xM[1];
        f[2] += -1.5*mu*0.001082626*pow(_RE,2)/pow(rM,5)*(3.0 - 5.*pow(xM[2]/rM,2)) * xM[2];
    }

    //Расчет вторых производных от притяжения третьего тела
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            long double E = 0.0;
            if (i==j) E = 1.0;
            f2[i][j] = f2[i][j] + mu/D3*(3.*(xM[i] - x[i])*(xM[j] - x[j])/D2 - E);
        }
    }
}

void Perturbations::geopot(long double _t, QVector<long double> x, QVector<long double>& f, QVector<QVector<long double>>& f2){
    eop_t EOP;
    QVector<long double> CONX(3);
    QVector<QVector<long double>> AR(3, QVector<long double>(3));
    QVector<QVector<long double>> CONX2(3, QVector<long double>(3));

    double dUT1, Xp,Yp;
    double rc2t[3][3];   //матрица поворота
    long double epochUTC_JD, epochTT_JD, epochUT1_JD, epochTai_JD;
    int dUtcTai;
    //получение времени UTC

    //примерная эпоха таи
    QDateTime curEpochTai = _beginEpochTai.addSecs(static_cast<int>(_t));
    //актуальная поправка между таи и утс
    dUtcTai = TimeSystem::leapSeconds(curEpochTai.date().year(), curEpochTai.date().month());
    //примерная эпоха утс // нужна для получения координат полюса
    QDateTime curEpochUTC = curEpochTai.addSecs(-dUtcTai);

    //координаты полюса и расхождение шкал времени
    _earthOrientationParams.getEop(curEpochUTC, EOP);
    dUT1 = EOP.dUt1;
    Xp = EOP.xp;
    Yp = EOP.yp;

    //точная эпоха TAI в юлианских днях
    epochTai_JD = static_cast<long double>(_beginEpochTai.date().toJulianDay()) - 0.5;
    epochTai_JD += _beginEpochTai.time().hour()/24.0 +
                   _beginEpochTai.time().minute()/1440.0 +
                  (_beginEpochTai.time().second() + _t)/86400.0;

    //точная эпоха UTC в юлианских днях
    epochUTC_JD = epochTai_JD - dUtcTai/86400.0;

    //точная эпоха TT в юлианских днях
    epochTT_JD = epochTai_JD + 32.184/86400.0;

    //получение эпохи UT1 в юлианских днях
    epochUT1_JD = epochUTC_JD + dUT1/86400.0;
    //получение матрицы поворота
    iauC2t06a(epochTT_JD, 0.0, epochUT1_JD, 0.0, Xp, Yp, rc2t);

    //алгоритм Канненгема
    for (int i=0; i<3; i++){
        for (int j=0; j<3; j++){
            AR[i][j] = rc2t[i][j];
        }
    }

    //cout << "-------" << endl;

    cangem(x,AR,CONX,CONX2);

    f[0] += CONX[0];
    f[1] += CONX[1];
    f[2] += CONX[2];

    // для вычисления производных вариациями
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            f2[i][j] += CONX2[i][j];
        }
    }

    CONX.clear(); AR.clear(); CONX2.clear();

}

void Perturbations::cangem(QVector<long double> X,
                           QVector<QVector<long double>> AR,
                           QVector<long double> &CONX,
                           QVector<QVector<long double>>& CONX2){
    long double CC[2], CP[2], CQ[2], CONX22[3][3];
    long double XR[3], VX[3], GF[3], GF2[3][3], VX2[3][3];

    long double GE = 398600.4418, RE = 6378.1366;
    long double GNORM = GE/(RE*RE);                             //тоже вынести в параметры констант и полей класса

    RE = 6378.1363;
    GE = 398600.4415;

    int NM = _NM;
    int MN = _MN;
    long double XV[2][3][NM+5][NM+5];
    long double R;
    long double R2 = 0.0;
    long double VV = 0.0;

    //обнуление XV
    for (int i=0; i<2; i++){
        for (int j=0; j<3; j++){
            for (int k=0; k<NM+5; k++){
                for (int l=0; l<NM+5; l++){
                    XV[i][j][k][l] = 0.0;
                }
            }
        }
    }

    for (int i=0; i<3; i++){
        CONX[i]=0.0;
        VX[i] = 0.0;
        GF[i] = 0.0;
        R2 += (X[i]/RE)*(X[i]/RE);
    }

    R = sqrt(R2);

    long double SINF = 1.0/R, OR2 = 1.0/R2, VC;
    CV[0][0][0] = SINF;

    for (int i=0; i<3; i++){
        VC = 0.0;
        for (int j=0; j<3; j++){
            VC += AR[i][j]*X[j];
        }
        //ПРЕОБРАЗОВАНИЕ КООРДИНАТ: XR = AR*X/R^2
        XR[i]=VC*OR2/RE;
    }

    int IP1, IM1, NM1, MM,JP1,JM1;
    long double SQ1, SQ2, SQ3, CS, SS;

    for (int I=1; I<NM; I++){
        IM1 = I-1;
        CP[0] = CV[0][IM1][IM1];
        CP[1] = CV[1][IM1][IM1];
        SQ2 = Q[I][I];
        CQ[0] = SQ2*(XR[0]*CP[0] - XR[1]*CP[1]);
        CQ[1] = SQ2*(XR[0]*CP[1] + XR[1]*CP[0]);
        //ВЫЧИСЛЕНИЕ ДИАГОНАЛЬНЫХ ФУНКЦИЙ ТИПА V_NM
        CV[0][I][I] = CQ[0];
        CV[1][I][I] = CQ[1];
    }
    //ШАРОВАЯ ФУНКЦИЯ V_10
    CV[0][1][0] = Q[1][0]*XR[2]*SINF;


    for (int I=2; I<NM; I++){
        IM1 = I-1;
        for (int J=0; J<=IM1; J++){
            CP[0] = CV[0][IM1][J];
            CP[1] = CV[1][IM1][J];
            CC[0] = CV[0][IM1-1][J];
            CC[1] = CV[1][IM1-1][J];
            SQ1 = Q[I][J]*XR[2];
            SQ2 = Q[J][I]*OR2;
            CQ[0] = SQ1*CP[0] - SQ2*CC[0];
            CQ[1] = SQ1*CP[1] - SQ2*CC[1];
            //ВЫЧИСЛЕНИЕ ОСТАЛЬНЫХ ФУНКЦИЙ V_NM (M<N)
            CV[0][I][J]=CQ[0];
            CV[1][I][J]=CQ[1];
        }
    }


    NM1= NM - 1;
    for (int I=1; I<NM1; I++){
        IP1 = I + 1;
        //КОЭФФИЦИЕНТ С_{I-1,0}
        CC[0] = CV[0][0][IP1];
        CC[1] = CV[1][0][IP1];

        SQ1 = QX[I][0]*CC[0];
        SQ2 = CC[0];
        SQ3 = QZ[I][0]*CC[0];

        CP[0] = CV[0][IP1][1];      //ШАРОВАЯ ФУНКЦИЯ V_{I,1}
        CP[1] = CV[1][IP1][1];      //ШАРОВАЯ ФУНКЦИЯ V_{I,1}
        CQ[0] = CV[0][IP1][0];      //ШАРОВАЯ ФУНКЦИЯ V_{I,0}
        CQ[1] = CV[1][IP1][0];      //ШАРОВАЯ ФУНКЦИЯ V_{I,0}
        CC[0] = CV[0][I][0];        //ШАРОВАЯ ФУНКЦИЯ V_{I-1,0}
        CC[1] = CV[1][I][0];        //ШАРОВАЯ ФУНКЦИЯ V_{I-1,0}

        XV[0][0][I][0] = -QX[I][0]*CP[0];
        XV[1][1][I][0] = -QX[I][0]*CP[1];
        XV[0][2][I][0] = -QZ[I][0]*CQ[0];

        if (I<NM-2) {
//        if (I<=NM-2) {   //посмотреть внимательно условия
            VX[0] = VX[0] - SQ1*CP[0];
            VX[1] = VX[1] - SQ1*CP[1];
            VX[2] = VX[2] - SQ3*CQ[0];
        }
        VV = VV + SQ2*CC[0];
    }


    for (int I=1; I<NM1; I++){
        IP1 = I+1;
        MM = I;
        for (int J = 1; J <= MM; J++) {
            if (J>=MN && I==NM-2) goto mark6;
            JP1 = J+1;
            JM1 = J-1;
            CC[0] = CV[0][J][IP1];
            CC[1] = CV[1][J][IP1];
            CS = CC[0];
            SS = CC[1];
            CP[0] = CV[0][IP1][JP1];
            CP[1] = CV[1][IP1][JP1];
            CC[0] = CV[0][IP1][J];
            CC[1] = CV[1][IP1][J];
            CQ[0] = CV[0][IP1][JM1];
            CQ[1] = CV[1][IP1][JM1];
            SQ1 = QX[I][J]*0.5;
            SQ2 = QX[JM1][I]*0.5;
            SQ3 = QZ[I][J];
            CP[0] = CP[0]*SQ1;
            CP[1] = CP[1]*SQ1;
            CQ[0] = CQ[0]*SQ2;
            CQ[1] = CQ[1]*SQ2;

            XV[0][0][I][J] = CQ[0] - CP[0];
            XV[1][0][I][J] = CQ[1] - CP[1];

            XV[0][1][I][J] = -(CQ[1] + CP[1]);
            XV[1][1][I][J] = CQ[0] + CP[0];

            XV[0][2][I][J] = -SQ3*CC[0];
            XV[1][2][I][J] = -SQ3*CC[1];

            if (J<=MN-2 && I<=NM-3) { //посмотреть повнимательнее при различных nm mn
                GF[0] = GF[0] + (CS*(CQ[0]-CP[0])+SS*(CQ[1]-CP[1]));
                GF[1] = GF[1] + (SS*(CQ[0]+CP[0])-CS*(CQ[1]+CP[1]));
                GF[2] = GF[2] - SQ3*(CS*CC[0]+SS*CC[1]);
            }
        }
    }


    //ВЫЧИСЛЕНИЕ СЕКТОРИАЛЬНОЙ И ТЕССАРАЛЬНОЙ ЧАСТИ РАЗЛОЖЕНИЯ
    mark6:

    long double CNM;
    for (int i=0; i<3; i++){
        CNM = 0.0;
        for (int j=0; j<3; j++){
            CNM += (VX[j] + GF[j])*AR[j][i];
        }
        //ПРЕДСТАВЛЕНИЕ ПОТЕНЦИАЛЬНЫХ СИЛ
        CONX[i] = CNM*GNORM;                   //данные должны быть из InitConst
    }

    //для вторых произвоных
    for (int i=0; i<3; i++){
        for (int j=0; j<3; j++){
            VX2[i][j] = 0.0;
            GF2[i][j] = 0.0;
            CONX2[i][j] = 0.0;
        }
    }

    //нормирование XV
    for (int i=0; i<2; i++){
        for (int j=0; j<3; j++){
            for (int k=0; k<NM+5; k++){
                for (int l=0; l<NM+5; l++){
                    XV[i][j][k][l] = XV[i][j][k][l]/RE;
                }
            }
        }
    }


    //заполнение VX2
    int NM2 = NM-2;
    for (int I=1; I<NM2; I++){
        IP1 = I + 1;

        CC[0] = CV[0][0][IP1];
        CC[1] = CV[1][0][IP1];

        SQ1 = QX[I][0]*CC[0];
        SQ3 = QZ[I][0]*CC[0];

        CP[0] = XV[0][0][IP1][1];
        CP[1] = XV[1][0][IP1][1];

        CQ[0] = XV[0][1][IP1][1];
        CQ[1] = XV[1][1][IP1][1];

        CC[0] = XV[0][2][IP1][1];
        CC[1] = XV[1][2][IP1][1];

        VX2[0][0] = VX2[0][0] - SQ1*CP[0];
        VX2[0][1] = VX2[0][1] - SQ1*CQ[0];
        VX2[0][2] = VX2[0][2] - SQ1*CC[0];

        VX2[1][0] = VX2[0][1];
        VX2[1][1] = VX2[1][1] - SQ1*CQ[1];
        VX2[1][2] = VX2[1][2] - SQ1*CC[1];

        VX2[2][0] = VX2[0][2];
        VX2[2][1] = VX2[1][2];
        VX2[2][2] = VX2[2][2] - SQ3*XV[0][2][IP1][0];
    }

    //заполнение GF2
    for (int I=1; I<NM2; I++){
        IP1 = I + 1;
        MM = I;

        for (int J=1; J<=MM; J++){
            if (J<=MN && I!=NM1) {
                JP1 = J + 1;
                JM1 = J - 1;
                CC[0] = CV[0][J][IP1];
                CC[1] = CV[1][J][IP1];
                CS = CC[0];
                SS = CC[1];
                //[0][0]
                CP[0] = XV[0][0][IP1][JP1];
                CP[1] = XV[1][0][IP1][JP1];
                CQ[0] = XV[0][0][IP1][JM1];
                CQ[1] = XV[1][0][IP1][JM1];
                SQ1 = QX[I][J]*0.5;
                SQ2 = QX[JM1][I]*0.5;
                CP[0] = CP[0]*SQ1;
                CP[1] = CP[1]*SQ1;
                CQ[0] = CQ[0]*SQ2;
                CQ[1] = CQ[1]*SQ2;
                GF2[0][0] = GF2[0][0] + CS*(CQ[0]-CP[0]) + SS*(CQ[1]-CP[1]);
                //[0][1]
                CP[0] = XV[0][1][IP1][JP1];
                CP[1] = XV[1][1][IP1][JP1];
                CQ[0] = XV[0][1][IP1][JM1];
                CQ[1] = XV[1][1][IP1][JM1];
                SQ1 = QX[I][J]*0.5;
                SQ2 = QX[JM1][I]*0.5;
                CP[0] = CP[0]*SQ1;
                CP[1] = CP[1]*SQ1;
                CQ[0] = CQ[0]*SQ2;
                CQ[1] = CQ[1]*SQ2;
                GF2[0][1] = GF2[0][1] + CS*(CQ[0]-CP[0]) + SS*(CQ[1]-CP[1]);
                //[0][2]
                CP[0] = XV[0][2][IP1][JP1];
                CP[1] = XV[1][2][IP1][JP1];
                CQ[0] = XV[0][2][IP1][JM1];
                CQ[1] = XV[1][2][IP1][JM1];
                SQ1 = QX[I][J]*0.5;
                SQ2 = QX[JM1][I]*0.5;
                CP[0] = CP[0]*SQ1;
                CP[1] = CP[1]*SQ1;
                CQ[0] = CQ[0]*SQ2;
                CQ[1] = CQ[1]*SQ2;
                GF2[0][2] = GF2[0][2] + CS*(CQ[0]-CP[0]) + SS*(CQ[1]-CP[1]);
                //[1][0]
                GF2[1][0] = GF2[0][1];
                //[1][1]
                CP[0] = XV[0][1][IP1][JP1];
                CP[1] = XV[1][1][IP1][JP1];
                CQ[0] = XV[0][1][IP1][JM1];
                CQ[1] = XV[1][1][IP1][JM1];
                SQ1 = QX[I][J]*0.5;
                SQ2 = QX[JM1][I]*0.5;
                CP[0] = CP[0]*SQ1;
                CP[1] = CP[1]*SQ1;
                CQ[0] = CQ[0]*SQ2;
                CQ[1] = CQ[1]*SQ2;
                GF2[1][1] = GF2[1][1] - CS*(CQ[1]+CP[1]) + SS*(CQ[0]+CP[0]);
                //[1][2]
                CP[0] = XV[0][2][IP1][JP1];
                CP[1] = XV[1][2][IP1][JP1];
                CQ[0] = XV[0][2][IP1][JM1];
                CQ[1] = XV[1][2][IP1][JM1];
                SQ1 = QX[I][J]*0.5;
                SQ2 = QX[JM1][I]*0.5;
                CP[0] = CP[0]*SQ1;
                CP[1] = CP[1]*SQ1;
                CQ[0] = CQ[0]*SQ2;
                CQ[1] = CQ[1]*SQ2;
                GF2[1][2] = GF2[1][2] - CS*(CQ[1]+CP[1]) + SS*(CQ[0]+CP[0]);
                //[2][0]
                GF2[2][0] = GF2[0][2];
                //[2][1]
                GF2[2][1] = GF2[1][2];
                //[2][2]
                CP[0] = XV[0][2][IP1][J];
                CP[1] = XV[1][2][IP1][J];
                SQ3 = QZ[I][J];
                CP[0] = CP[0]*SQ3;
                CP[1] = CP[1]*SQ3;
                GF2[2][2] = GF2[2][2] - CS*SQ3*CP[0] + SS*SQ3*CP[1];
            } //if
        }// for J
    }// for I

    for (int i=0; i<3; i++){
        for (int j=0; j<3; j++){
            CONX22[i][j] = (VX2[i][j]+GF2[i][j])*GNORM;
        }
    }
    long double SNM;
    for (int N = 0; N < 3; N++){
        for (int K=0; K < 3; K++){
            CNM = 0.0;
            for (int I = 0; I < 3; I++) {
                SNM = 0.0;
                for (int J = 0; J < 3; J++) {
                    SNM = SNM + CONX22[I][J]*AR[J][K];
                }
                CNM = CNM + SNM*AR[I][N];
            }
            //представление производных от потенциальных сил
            CONX2[K][N] = CNM + CONX2[K][N];
        }
    }

}

void Perturbations::initGeoConst(){
    //Заполнение CV,Q,QX,QZ
    for (int k=0; k<2; k++){
        for (int i=0; i<25; i++){
            for (int j=0; j<25; j++){
                CV[k][i][j] = InitConst::CV[k][i][j];

            }
        }
    }

    for (int i=0; i<23; i++){
        for (int j=0; j<23; j++){
            Q[i][j] = InitConst::Q[i][j];
            QX[i][j] = InitConst::QX[i][j];
            QZ[i][j] = InitConst::QZ[i][j];
        }
    }

    if (_isTides){
        for (int i=0;i<23; i++){
            for (int j=0; j<24; j++){
                CNST[i][j] = InitConst::CNST[i][j] ;
                SNST[i][j] = InitConst::SNST[i][j] ;
            }
        }
    }


}

void Perturbations::initEop(QDateTime beginEpochUtc, QDateTime endEpochUtc){
    _earthOrientationParams.getListEop(beginEpochUtc,endEpochUtc);
}

void Perturbations::changeGarmCoef(long double _tTaiSec)
{
    const long double DAS2R = 4.848136811095359935899141E-6;
    long double TJD, TJD2010, TJD2000, DEL_T, XP, YP;
    long double XPU2010[4] = {0.055974, 0.0018243, 0.00018413, 0.000007024};
    long double YPU2010[4] = {0.346346, 0.0017896,-0.00010729,-0.000000908};
    long double XPA2010[4] = {0.023513, 0.0076141, 0.0, 0.0};
    long double YPA2010[4] = {0.358891,-0.0006287, 0.0, 0.0};
    int UTC2TAI;

    // перевод времени в TT юлианских дней
    TJD = static_cast<long double>(_beginEpochTai.date().toJulianDay()) - 0.5;
    TJD = TJD +(_beginEpochTai.time().hour()*3600.0 +
                _beginEpochTai.time().minute()*60.0 +
                _beginEpochTai.time().second() + _tTaiSec)/86400.0;
    TJD = TJD + 32.184/86400.0;

    //получение времени в ТТ
    UTC2TAI = TimeSystem::leapSeconds(2010, 1);

    TJD2010 = 2454832.5 + (static_cast<long double>(UTC2TAI) + 32.184)/86400.0;  // 12:00:00, 01.01.2009 year

//    TJD2010 = 2455197.5 + (static_cast<long double>(UTC2TAI) + 32.184)/86400.0;  // 12:00:00, 01.01.2010 year

    UTC2TAI = TimeSystem::leapSeconds(2000, 1);

    TJD2000 = 2451545.5 + (static_cast<long double>(UTC2TAI) + 32.184)/86400.0;  // 12:00:00, 01.01.2000 J2000.0

    DEL_T = (TJD - TJD2000)/365.25;
    XP = 0.0;
    YP = 0.0;

    // in second of arc
    if (TJD > TJD2010) {
        for (int i=0; i<4; i++){
            XP = XP + pow(DEL_T,i)*XPA2010[i];
            YP = YP + pow(DEL_T,i)*YPA2010[i];
        }
    }
    else {
        for (int i=0; i<4; i++){
            XP = XP + pow(DEL_T,i)*XPU2010[i];
            YP = YP + pow(DEL_T,i)*YPU2010[i];
        }
    }

    // in rad
    XP = XP*DAS2R;
    YP = YP*DAS2R;

    CV[0][0][3] = CNST[1][0] + DEL_T*11.62755E-12;                            // C20
    CV[0][1][3] = sqrt(3.)*XP*CV[0][0][3] - XP*CV[0][2][3] + YP*CV[1][2][3];  // C21
    CV[1][1][3] =-sqrt(3.)*YP*CV[0][0][3] - YP*CV[0][2][3] - XP*CV[1][2][3];  // S21

}

void Perturbations::tides(QVector<long double> x, QVector<long double> xMoon, QVector<long double> xSun, QVector<long double> &f)
{
    long double pTides[3], DK, rMoon, rx, rSun, COSPHI;

    //растояние до ИСЗ
    rx = sqrt(x[0]*x[0] + x[1]*x[1] + x[2]*x[2]);
    //расстояние от Земли до Луны
    rMoon = sqrt(xMoon[0]*xMoon[0] + xMoon[1]*xMoon[1] + xMoon[2]*xMoon[2]);
    //расстояние от Земли до Солнца
    rSun = sqrt(pow(xSun[0],2) + pow(xSun[1],2) + pow(xSun[2],2));

    DK = 0.298;

    COSPHI = (x[0]*xMoon[0] + x[1]*xMoon[1] + x[2]*xMoon[2])/(rx*rMoon);

    pTides[0] = DK*_GM*pow(_RE,5)/(pow(rMoon,3)*pow(rx,4))*(3.*COSPHI*xMoon[0]/rMoon - (7.5*pow(COSPHI,2) - 1.5)*x[0]/rx);
    pTides[1] = DK*_GM*pow(_RE,5)/(pow(rMoon,3)*pow(rx,4))*(3.*COSPHI*xMoon[1]/rMoon - (7.5*pow(COSPHI,2) - 1.5)*x[1]/rx);
    pTides[2] = DK*_GM*pow(_RE,5)/(pow(rMoon,3)*pow(rx,4))*(3.*COSPHI*xMoon[2]/rMoon - (7.5*pow(COSPHI,2) - 1.5)*x[2]/rx);

    COSPHI = (x[0]*xSun[0] + x[1]*xSun[1] + x[2]*xSun[2])/(rx*rSun);

    pTides[0] = pTides[0] + DK*_GS*pow(_RE,5)/(pow(rSun,3)*pow(rx,4))*(3.*COSPHI*xSun[0]/rSun - (7.5*pow(COSPHI,2) - 1.5)*x[0]/rx);
    pTides[1] = pTides[1] + DK*_GS*pow(_RE,5)/(pow(rSun,3)*pow(rx,4))*(3.*COSPHI*xSun[1]/rSun - (7.5*pow(COSPHI,2) - 1.5)*x[1]/rx);
    pTides[2] = pTides[2] + DK*_GS*pow(_RE,5)/(pow(rSun,3)*pow(rx,4))*(3.*COSPHI*xSun[2]/rSun - (7.5*pow(COSPHI,2) - 1.5)*x[2]/rx);

    f[0] += pTides[0];
    f[1] += pTides[1];
    f[2] += pTides[2];
}

void Perturbations::read438_16(long double _t, QVector<long double> &xM, QVector<long double> &xS, QVector<QVector<long double> > &xPLS)
{
    const __float128 ratl = 1.0 / (81.30056833816509 + 1.0);
    const int ndbl = 818;                                               // кол-во чисел (две временные отметки и коэ-фты)
    const int nrsize = ndbl * sizeof (__float128);                     // размер записи в байтах для одного блока данных
    const __float128 tmin = 2287184.5, tmax = 2688976.5;

    QVector<QVector<__float128>> xP(9, QVector<__float128>(6));
    QVector<__float128> xv(66), xE(6),buf16(ndbl), x_bar(6);
    int iop = 0;
    int nrc = -1;
    int isPlanets = 1;
    __float128 t;

    //перевод времени из относительных секунд ТАI в полную эпоху ТТ
    t = static_cast<__float128>(_beginEpochTai.date().toJulianDay()) - 0.5;
    t = t + static_cast<__float128>(_beginEpochTai.time().hour()/24.0)
          + static_cast<__float128>(_beginEpochTai.time().minute()/1440.0)
          + static_cast<__float128>(_beginEpochTai.time().second()/86400.0);
    t = t + _t/86400.0;
    t = t + 32.184/86400.0;      //перевод из TAI в ТТ

    //перевод времени из ТТ в TDB
//    __float128 dTDB, g;
//    if (false) {
//        g = (357.528*RAD + 35999.050*RAD*((t-2451545.0)/36525.0))*3.1415926535897932384626433832795 / 180.;
//        dTDB = (0.001658/86400.)*sinq(g+0.0167*sinq(g));  //поправка ко времени
//        t = t + dTDB;
//    }



    // Проверка границ времени
    if (t < tmin || t > tmax) {         // Время для считывания с фонда в юлианских сутках, t
        cout << "date outside the foundation (1550-2650 гг.)" << endl;
        return;
    }

    // Чтение данных из бинарного файла
    if (iop == 0) {
        ifstream file(fondFileName, ios::binary);                      //
//        ifstream file("fond438.bin", ios::binary);                      //

        if (!file.is_open()) {
            cerr << "file openning error" << endl;

            if (file.bad()){
                cerr << "Fatal Error: badbit is set" << endl;
            }

            if (file.fail()) {
                cerr << "Error details: " << strerror(errno)<< endl;
            }
            return;
        }

        // Рассчитываем смещение для нужной записи (NR)
        int nr = (t - tmin) / 32 + 1;
        //int nr = static_cast<int>((t - tmin) / 32)+ 1;


        // Смещаем указатель файла на нужную запись
        file.seekg((nr - 1) * nrsize, ios::beg);  // Смещаем указатель на NR-ую запись
        if (file.fail()) {
            cerr << "Position error in the file" << endl;
            file.close();
            return;
        }

        // Чтение данных в буфер
        __float128 num;
        for (int i=0; i<ndbl; i++){
            file.read((char*)&num, sizeof (__float128));
            buf16[i] = num;
        }
        if (file.fail()) {
            cerr << "file reading error" << endl;
            file.close();
            return;
        }
        file.close();

        nrc = nr;
    }


    // Проверка корректности считанной записи
    if (t < buf16[0] || t > buf16[1]) {                 //первые две записи должны содержать временные отметки с интервалом в 32 суток.
        qDebug() << QString::number(buf16[0],'f',10);  //TODO:
        qDebug() << QString::number(buf16[1],'f',10);

        cerr << "the wrong entry was read" << endl;
        return;
    }


    // Вычисление координат с использованием COOR438 для каждого тела
    for (int i = 0 ; i < 11; i++) {
        //вычисление координат в барицентрической СК солн.системы (кроме Луны)
        coor438_16(i, t, buf16, x_bar);
        for (int j=0; j<6; j++){
            xv[6*i + j] = x_bar[j]; // заполнение массива xv
        }
    }

    //координаты и скорости Луны
    xM = {static_cast<long double>(xv[54]),
          static_cast<long double>(xv[55]),
          static_cast<long double>(xv[56]),
          static_cast<long double>(xv[57]),
          static_cast<long double>(xv[58]),
          static_cast<long double>(xv[59])};

    //только положения Солнца
    if (isPlanets == 0) {

        for (int i = 0; i < 6; i++) {
            xE[i] = xv[12 + i] - ratl * xv[54 + i];
        }

        for (int i = 0; i < 6; i++) {
            xE[i] = xE[i] - xv[60 + i];
        }

        xS = {static_cast<long double>(-xE[0]),
              static_cast<long double>(-xE[1]),
              static_cast<long double>(-xE[2]),
              static_cast<long double>(-xE[3]),
              static_cast<long double>(-xE[4]),
              static_cast<long double>(-xE[5])};
    }
    //положение Солнца и других планет
    else {


        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 6; j++) {
                //поправка на влияние луны?
                xP[i][j] = xv[6 * i + j] - ratl * xv[54 + j];
                //перевод из барицентрической в гелиоцентрическую СК
                xP[i][j] = xP[i][j] - xv[60 + j];
            }
        }

        //положения солнца
        for (int j = 0; j < 6; ++j) {
            xS[j] = -xP[2][j];
        }

        //выходные данные с массивом координат планет
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 3; j++) {
                //перевод из гелиоцентрической в геоцентрическую СК
                xPLS[i][j] = xP[i][j] - xP[2][j];
            }
        }
    }
}

void Perturbations::coor438_16(int i, __float128 t, QVector<__float128> buf16, QVector<__float128> &xc)
{
    // возвращает координаты планет, луны и солнца на конкретный момент времени
    // при помощи интерполяции полиномами чебышева
    // коэф-ты полиномов получены из фонда
    const __float128 secsut = 86400.0;

    QVector<__float128> tc(16), tcp(16);
    QVector<int> nper = {3, 171, 231, 309, 342, 366, 387, 405, 423, 441, 753};
    QVector<int> ipow = {13, 9, 12, 10, 7, 6, 5, 5, 5, 12, 10};
    QVector<int> ncf = {42, 30, 39, 0, 0, 0, 0, 0, 0, 39, 33};
    QVector<__float128> sut = {8.0, 16.0, 16.0, 32.0, 32.0, 32.0, 32.0, 32.0, 32.0, 4.0, 16.0};

    int nint = (t - buf16[0]) / sut[i];
    long double a = buf16[0] + nint * sut[i];
    int ist = ipow[i] + 1;
    long double b = a + sut[i];

    //вызов функции полиномов чебышева для tc, tcp
    cheb_16(a, b, t, ipow[i], tc, tcp);


    int jn = nper[i] + nint * ncf[i];

    //для координат
    for (int k = 0; k < 3; k++) {
        xc[k] = 0.0;
        int jj = jn + k * ist;
        for (int j = 0; j <= ipow[i]; j++) {
            xc[k] += tc[j] * buf16[jj + j-1];
        }
    }

    //для скоростей
    for (int k = 0; k < 3; k++) {
        xc[k+3] = 0.0;
        int jj = jn + k * ist;
        for (int j = 1; j <= ipow[i]; j++) {
            xc[k+3] += tcp[j] * buf16[jj + j-1];
        }
    }
    //нормирование
    xc[3] = xc[3]/secsut;
    xc[4] = xc[4]/secsut;
    xc[5] = xc[5]/secsut;
}

void Perturbations::cheb_16(__float128 a, __float128 b, __float128 t, int ist, QVector<__float128> &tc, QVector<__float128> &tcp)
{
    const __float128 two = 2.0, one = 1.0, four = 4.0;
    __float128 dlin = b - a;
    __float128 tau = two * (t - a) / dlin - one;
    __float128 tau2 = tau * two;

    tc[0] = one;
    tc[1] = tau;

    //узловые значения полинома чебышева для координат
    for (int i = 2; i <= ist; i++) {
        tc[i] = tau2 * tc[i - 1] - tc[i - 2];
    }

    __float128 rat = four / dlin;
    tcp[0] = 0.0;
    tcp[1] = two / dlin;

    //узловые значения полинома чебышева для скоростей
    for (int i = 2; i <= ist; i++) {
        tcp[i] = rat * tc[i - 1] + tau2 * tcp[i - 1] - tcp[i - 2];
    }
}

void Perturbations::read438(long double _t,QVector<long double> &xM,QVector<long double> &xS,QVector<QVector<long double>> &xPlanets){
    //время _t должно быть в секундах
    long double t;
    const long double ratl = 1.0 / (81.30056833816509 + 1.0);
    const int ndbl = 818;                                               // кол-во чисел (две временные отметки и коэ-фты)
    const int nrsize = ndbl * sizeof (long double);                     // размер записи в байтах для одного блока данных
    const long double tmin = 2287184.5, tmax = 2688976.5;
    const long double RAD (3.14159265358979323846 / 180.);
    long double xP[9][6];                                               // промежуточный массив для координат в гелиоцентрической
    long double xv[66], xE[6];
    QVector<long double> buf16(ndbl), x_bar(6);
    int iop = 0;
    int nrc = -1;
    long double g, dTDB;

    //перевод времени из относительных секунд ТАI в полную эпоху ТТ
    t = static_cast<long double>(_beginEpochTai.date().toJulianDay()) - 0.5;
    t = t + _beginEpochTai.time().hour()/24.0 + _beginEpochTai.time().minute()/1440.0 + _beginEpochTai.time().second()/86400.0;
    t = t + _t/86400.0;
    t = t + 32.184/86400.0;      //перевод из TAI в ТТ

    //перевод времени из ТТ в TDB
    if (false) {
        g = (357.528*RAD + 35999.050*RAD*((t-2451545.0)/36525.0))*RAD;
        dTDB = (0.001658/86400.)*sin(g+0.0167*sin(g));  //поправка ко времени
        t = t + dTDB;
    }

    // Проверка границ времени
    if (t < tmin || t > tmax) {                                         // Время для считывания с фонда в юлианских сутках, по шкале (TDB)
        cout << "date outside the foundation (1550-2650 гг.)" << endl;
        return;
    }
    // Чтение данных из бинарного файла
    if (iop == 0) {
        ifstream file(fondFileName, ios::binary);                      //указать корректный путь до фонда
        if (!file.is_open()) {
            cerr << "file openning error" << endl;

            if (file.bad()){
                cerr << "Fatal Error: badbit is set" << endl;
            }

            if (file.fail()) {
                cerr << "Error details: " << strerror(errno)<< endl;
            }
            return;
        }

        // Рассчитываем смещение для нужной записи (NR)
        int nr = (t - tmin) / 32 + 1;
        //cout << " T : " <<  _t << "    " <<  nr << " " << t << " " << tmin << endl;

        // Смещаем указатель файла на нужную запись
        file.seekg((nr - 1) * nrsize, ios::beg);  // Смещаем указатель на NR-ую запись

        if (file.fail()) {
            cerr << "Position error in the file" << endl;
            file.close();
            return;
        }

        // Чтение данных в буфер
        long double num;
        for (int i=0; i<ndbl; i++){
            file.read((char*)&num, sizeof (long double));
            buf16[i] = num;
        }
        if (file.fail()) {
            cerr << "file reading error" << endl;
            file.close();
            return;
        }
        file.close();

        nrc = nr;
    }

    // Проверка корректности считанной записи
    if (t < buf16[0] || t > buf16[1]) {                 //первые две записи должны содержать временные отметки с интервалом в 32 суток.
        qDebug() << QString::number(buf16[0],'f',10);
        qDebug() << QString::number(buf16[1],'f',10);

        cerr << "the wrong entry was read" << endl;
        return;
    }


    // Вычисление координат с использованием COOR438 для каждого тела
    for (int i = 0 ; i < 11; i++) {
        //вычисление координат в барицентрической СК солн.системы (кроме Луны)
        coor438(i, t, buf16, x_bar);
        for (int j=0; j<6; j++){
            xv[6*i + j] = x_bar[j]; // заполнение массива xv
        }
    }

    //координаты и скорости Луны
    xM[0] = static_cast<long double>(xv[54]);
    xM[1] = static_cast<long double>(xv[55]);
    xM[2] = static_cast<long double>(xv[56]);
    xM[3] = static_cast<long double>(xv[57]);
    xM[4] = static_cast<long double>(xv[58]);
    xM[5] = static_cast<long double>(xv[59]);

    //только положения Солнца
    if (_isPlanets == 0) {

        for (int i = 0; i < 6; i++) {
            xE[i] = xv[12 + i] - ratl * xv[54 + i];
        }

        for (int i = 0; i < 6; i++) {
            xE[i] = xE[i] - xv[60 + i];
        }
        for (int i = 0; i < 6; i++){
            xS[i] = -xE[i];
        }
    }
    //положение Солнца и других планет
    else {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 6; j++) {
                //переход из барицентра Земля-Луна к координатам Земли
                xP[i][j] = xv[6 * i + j] - ratl * xv[54 + j];
                //перевод из барицентрической в гелиоцентрическую СК
                xP[i][j] = xP[i][j] - xv[60 + j];
            }
        }
        //положения солнца
        for (int j = 0; j < 6; ++j) {
            xS[j] = -xP[2][j];
        }
        //выходные данные с массивом координат планет
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 3; j++) {
                //перевод из гелиоцентрической в геоцентрическую СК
                xPlanets[i][j] = xP[i][j] - xP[2][j];
            }
        }
    }

    buf16.clear(); x_bar.clear();
}

void Perturbations::coor438(int i, long double t, QVector<long double> buf16,  QVector<long double>& xc){
    // возвращает координаты планет, луны и солнца на конкретный момент времени
    // при помощи интерполяции полиномами чебышева
    // коэф-ты полиномов получены из фонда
    const long double secsut = 86400.0;

    QVector<long double> tc(16), tcp(16);
    int nper[11] = { 3, 171,  231, 309, 342, 366, 387, 405, 423, 441, 753};
    int ipow[11] = {13,   9,   12,  10,   7,   6,   5,   5,   5,  12, 10};
    int ncf[11] = { 42,  30, 39, 0, 0, 0, 0, 0, 0, 39, 33};
    long double sut[11] = {8.0, 16.0, 16.0, 32.0, 32.0, 32.0, 32.0, 32.0, 32.0, 4.0, 16.0};

    int nint = (t - buf16[0]) / sut[i];
    long double a = buf16[0] + nint * sut[i];
    int ist = ipow[i] + 1;
    long double b = a + sut[i];

    //вызов функции полиномов чебышева для tc, tcp
    cheb(a, b, t, ipow[i], tc, tcp);


    int jn = nper[i] + nint * ncf[i];

    //для координат
    for (int k = 0; k < 3; k++) {
        xc[k] = 0.0;
        int jj = jn + k * ist;
        for (int j = 0; j <= ipow[i]; j++) {
            xc[k] += tc[j] * buf16[jj + j-1];
        }
    }

    //для скоростей
    for (int k = 0; k < 3; k++) {
        xc[k+3] = 0.0;
        int jj = jn + k * ist;
        for (int j = 1; j <= ipow[i]; j++) {
            xc[k+3] += tcp[j] * buf16[jj + j-1];
        }
    }
    //нормирование
    xc[3] = xc[3]/secsut;
    xc[4] = xc[4]/secsut;
    xc[5] = xc[5]/secsut;

    tc.clear(); tcp.clear();

}

void Perturbations::cheb(long double a,long double b,long double t, int ist,
                         QVector<long double>& tc, QVector<long double>& tcp){
    //
    const long double two = 2.0, one = 1.0, four = 4.0;
    long double dlin = b - a;
    long double tau = two * (t - a) / dlin - one;
    long double tau2 = tau * two;

    tc[0] = one;
    tc[1] = tau;

    //узловые значения полинома чебышева для координат
    for (int i = 2; i <= ist; i++) {
        tc[i] = tau2 * tc[i - 1] - tc[i - 2];
    }

    long double rat = four / dlin;
    tcp[0] = 0.0;
    tcp[1] = two / dlin;

    //узловые значения полинома чебышева для скоростей
    for (int i = 2; i <= ist; i++) {
        tcp[i] = rat * tc[i - 1] + tau2 * tcp[i - 1] - tcp[i - 2];
    }
}

void Perturbations::rltvt(QVector<long double> x, QVector<long double> v,
                          QVector<long double> xSS, QVector<long double> &f)
{
    const long double W=9.7803278E2;
    const long double CL2=299792.458*299792.458;
    const long double RC2 = 1./CL2;
    long double DMUS = _GS;
    long double DMU = _GE;
    long double R, R2, R3, V2, RV, RS, RS2, COEF1, COEF2, C[3];
    long double PHI_0[3], PHI_1[3], PHI_2[3], P[3];
    long double xS[6];

    int IGPR,ISCHW,ILTER;
    IGPR = 1; ISCHW =1; ILTER = 1;


    for (int i=0; i<6; i++){
        xS[i] = -xSS[i];
    }

    for (int i=0; i<3; i++){
        PHI_0[i] = 0.0;
        PHI_1[i] = 0.0;
        PHI_2[i] = 0.0;
    }

    R2 = x[0]*x[0] + x[1]*x[1] + x[2]*x[2];
    R = sqrt(R2);
    R3 = R2*R;
    V2 = v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
    RV = x[0]*v[0] + x[1]*v[1] + x[2]*v[2];

    if (IGPR!=0) {
        RS2 = xS[0]*xS[0] + xS[1]*xS[1] + xS[2]*xS[2];
        RS = sqrt(RS2);
        COEF1 = 3.0 * DMUS/RS2/RS;
        //Вектор площадей солнца
        C[0] = xS[1]*xS[5] - xS[2]*xS[4];
        C[1] = xS[2]*xS[3] - xS[0]*xS[5];
        C[2] = xS[0]*xS[4] - xS[1]*xS[3];

        PHI_0[0] = COEF1*(C[1]*v[2] - C[2]*v[1]);
        PHI_0[1] = COEF1*(C[2]*v[0] - C[0]*v[2]);
        PHI_0[2] = COEF1*(C[0]*v[1] - C[1]*v[0]);
    }

    if (ISCHW!=0) {
        COEF1 = DMU/R3;
        PHI_1[0] = COEF1*((4.*DMU/R - V2)*x[0] + 4.*RV*v[0]);
        PHI_1[1] = COEF1*((4.*DMU/R - V2)*x[1] + 4.*RV*v[1]);
        PHI_1[2] = COEF1*((4.*DMU/R - V2)*x[2] + 4.*RV*v[2]);
    }

    if (ILTER!=0){
        COEF1 = 2.0*DMU*W/R3;
        COEF2 = 3.0*x[2]/R2;
        PHI_2[0] = COEF1*(COEF2*(x[1]*v[2] - x[2]*v[1])+v[1]);
        PHI_2[1] = COEF1*(COEF2*(x[2]*v[0] - x[0]*v[2])-v[0]);
        PHI_2[2] = COEF1*(COEF2*(x[0]*v[1] - x[1]*v[0]));
    }

    for (int i=0; i<3; i++){
        P[i] = (PHI_0[i] + PHI_1[i] + PHI_2[i])*RC2;
    }

    f[0] += P[0];
    f[1] += P[1];
    f[2] += P[2];

}

void Perturbations::radPressure2(QVector<long double> x0,
                                 QVector<long double> v0,
                                 QVector<long double> xS,
                                 QVector<long double> xM,
                                 QVector<long double> &f,
                                 QVector<QVector<long double> > &DFSL,
                                 QVector<QVector<long double> > &f2)
{
    long double var[3];
    QVector<long double> x(3), SP_PV(3), SP_MV(3);

    /*//Вариации параметров светового давления
    long double var_lc = 1e-12;
    long double var_lc1 = 1e-12;
    QVector<long double> f_p, f_l;
    long double df_1, df_2, df_3;

    for(int i = 0; i < 48; i++){
        f_p.append(0.0L);
        f_l.append(0.0L);
    }
    for(int i = 0; i < 3; i++){
        for(int i1 = 0; i1 < 48; i1++){
            f_p[i1] = f[i1];
        }
        B123[i][0] = B123[i][0] + var_lc;
        radPressure(x0,v0,xS,xM,f_p,DFSL);

        for(int i1 = 0; i1 < 48; i1++){
            f_l[i1] = f[i1];
        }
        B123[i][0] = B123[i][0] - 2.0*var_lc;
        radPressure(x0,v0,xS,xM,f_l,DFSL);
        B123[i][0] = B123[i][0] + var_lc;

        df_1 = (f_p[0] - f_l[0]) / (2.0 * var_lc);
        df_2 = (f_p[1] - f_l[1]) / (2.0 * var_lc);
        df_3 = (f_p[2] - f_l[2]) / (2.0 * var_lc);

        //if(df_1*DFSL[i+3][0] < 0 || df_2*DFSL[i+3][1] < 0 || df_3*DFSL[i+3][2] < 0*)
        if(abs(df_1 - DFSL[i+6][0]) > 1e-8 || abs(df_2 - DFSL[i+6][1]) > 1e-8 || abs(df_3 - DFSL[i+6][2]) > 1e-8){
            cout << " ------------------------------------ " << endl;
            cout << "A1" << i + 1 << endl;
            cout << df_1 << " " << DFSL[i+6][0] << " " << df_1 - DFSL[i+6][0] << endl;
            cout << df_2 << " " << DFSL[i+6][1] << " " << df_2 - DFSL[i+6][1] << endl;
            cout << df_3 << " " << DFSL[i+6][2] << " " << df_3 - DFSL[i+6][2] << endl;
            cout << " ------------------------------------ " << endl;
        }
    }*/


    //Вариации координат
    var[0] = 1e-5;
    var[1] = 1e-5;
    var[2] = 1e-5;

    for (int i = 0; i < 3; ++i) {
        x[0] = x0[0];
        x[1] = x0[1];
        x[2] = x0[2];

        SP_PV[0]=0.0;
        SP_PV[1]=0.0;
        SP_PV[2]=0.0;

        SP_MV[0]=0.0;
        SP_MV[1]=0.0;
        SP_MV[2]=0.0;

        x[i] = x0[i] + var[i];
        radPressure(x,v0,xS,xM,SP_PV,DFSL);

        x[i] = x0[i] - var[i];
        radPressure(x,v0,xS,xM,SP_MV,DFSL);

        f2[i][0] += (SP_PV[0]-SP_MV[0])/(2.0*var[i]);
        f2[i][1] += (SP_PV[1]-SP_MV[1])/(2.0*var[i]);
        f2[i][2] += (SP_PV[2]-SP_MV[2])/(2.0*var[i]);

    }
    radPressure(x0,v0,xS,xM,f,DFSL);

    x.clear(), SP_PV.clear(), SP_MV.clear();

}

void Perturbations::radPressure(QVector<long double> x,  QVector<long double> v,  //коор-ты, скорости
                                QVector<long double> xS, QVector<long double> xM, //коор-ты Солнца, Луны
                                QVector<long double> &f,
                                QVector<QVector<long double>>& DFSL)
{
    const long double PI = 3.1415926535897932384626433832795;
    const long double RZD_NEW = 6402.0;         // средний радиус Земли в новой МД, км
    const long double RSD = 695980.0;           // средний радиус Солнца, км
    const long double RMD = 1738.0;             // средний радиус Луны,  км
    const long double GS_NEW = 0.13271244E+12;

    long double R,R2, ROS, ROM, E1R, DXSX, DXS2, CDX, XS2, VECCM, VECCS;
    long double TETAS, PSI, CTETAL, FI, TETAL, CBETA, BETA, CALFA, ALFA, GAM;
    long double NU, NU1, NU2;
    long double CGAM, TERM, BUF, COSFI, SINFI, ARGLAT;

    QVector<long double> E1(3), E2(3), E3(3);  //орты
    QVector<long double> ER(3), EM(3), VECC(3),VECS(3), VECCN(3), VECP(3);

    long double DXS[3],DXM[3], KOEF[3];
    long double SP[3];
    int  IORDER;

    // БЛОК 1.0--------Расчет  ортонормированных векторов--------------------------
    DXS[0] = x[0] - xS[0];
    DXS[1] = x[1] - xS[1];      //вектор КА-Солнце
    DXS[2] = x[2] - xS[2];

    DXM[0] = x[0] - xM[0];
    DXM[1] = x[1] - xM[1];      //вектор КА-Луна
    DXM[2] = x[2] - xM[2];

    R = sqrt(x[0]*x[0] + x[1]*x[1] + x[2]*x[2]);
    //cout << "x = {" << x[0] << "; " << x[1] << "; " << x[2] << "}" << endl;
    //cout << "R: " << R << endl;
    ROS = sqrt(DXS[0]*DXS[0] + DXS[1]*DXS[1] + DXS[2]*DXS[2]);
    ROM = sqrt(DXM[0]*DXM[0] + DXM[1]*DXM[1] + DXM[2]*DXM[2]);

    E1[0] = DXS[0]/ROS;
    E1[1] = DXS[1]/ROS;         // E1
    E1[2] = DXS[2]/ROS;

    ER[0] = x[0]/R;
    ER[1] = x[1]/R;
    ER[2] = x[2]/R;

    EM[0] = DXM[0]/ROM;
    EM[1] = DXM[1]/ROM;         //направляющий вектор КА-Луна
    EM[2] = DXM[2]/ROM;

    // БЛОК 2.0-------Расчет  E1,E2,E3 – единичные вектора НПСК--------------------
    vecMul(E1,ER,E2);
    E1R = sqrt(E2[0]*E2[0] + E2[1]*E2[1] + E2[2]*E2[2]);

    E2[0] = E2[0]/E1R;
    E2[1] = E2[1]/E1R;          // E2
    E2[2] = E2[2]/E1R;

    vecMul(E1,E2,E3);           // E3

    // БЛОК 3.0-----------Расчет  угла СОЗ  -  tetas ------------------------------

    DXSX = DXS[0]*x[0] + DXS[1]*x[1] + DXS[2]*x[2];              // --КА-Солнце*r
    R2 = x[0]*x[0] + x[1]*x[1] + x[2]*x[2];                     // --модуль рад-вектора^2
    DXS2= DXS[0]*DXS[0] + DXS[1]*DXS[1] + DXS[2]*DXS[2];                             // --модуль КА-Солнце^2
    DXSX = DXSX/sqrt(R2)/sqrt(DXS2);
    TETAS = acos(DXSX);

    // БЛОК 4.0----------Расчет значение функции тени от Луны  - nu1---------------


    PSI = RSD / ROS;
    CTETAL = E1[0]*EM[0] + E1[1]*EM[1] + E1[2]*EM[2];

    if (CTETAL < 0.999390827019) {
        NU1 = 1.0;
    }
    else {
        FI = RMD/ROM;
        TETAL = PI/2.0 - asin(CTETAL);

        if (TETAL - PSI - FI >= 0.0) {
            NU1 = 1.0;
        }
        else {
            if (TETAL + PSI - FI<0.0){
                NU1 = 0.0;
            }
            else {
                if (TETAL - PSI + FI<0.0){
                    NU1 = (PSI*PSI - FI*FI)/(PSI*PSI);
                }
                else {
                    CBETA = (FI*FI - TETAL*TETAL - PSI*PSI)/(2.0*TETAL*PSI);
                    BETA = acos(CBETA);

                    CALFA = (TETAL + PSI*CBETA)/FI;
                    ALFA = acos(CALFA);

                    NU1 = BETA/PI + TETAL*sin(BETA)/(PI*PSI) - (ALFA*FI*FI)/(PI*PSI*PSI);
                }
            }
        }
    }

    // БЛОК 5.0 ------------Расчет функции тени от Земли - nu2---------------------

    FI = asin(RZD_NEW/R);
    //cout << "FI: " << FI << endl;
    if (TETAS - PSI - FI >= 0.0) {
        NU2 = 1.0;
    }
    else {
        if (TETAS + PSI - FI < 0.0) {
            NU2 = 0.0;
        }
        else {
            CBETA = (FI*FI - TETAS*TETAS - PSI*PSI)/(2.0*TETAS*PSI);
            BETA = acos(CBETA);

            CALFA = (TETAS+PSI*CBETA)/FI;
            ALFA = acos(CALFA);

            NU2 = BETA/PI + TETAS*sin(BETA)/(PI*PSI) - (ALFA*FI*FI)/(PI*PSI*PSI);

        }
    }
    //cout << "NU2 : " << NU2 << endl;

    NU = NU1*NU2;
    //cout << "NU: " << NU << endl;

    // БЛОК 6.0------------Расчет проекции ускорения от РД на КА-------------------
    vecMul(x,v,VECC);                                               // --вектор площадей
    CDX = VECC[0]*xS[0] + VECC[1]*xS[1] + VECC[2]*xS[2];            // --косинус угла между вектором площадей и вектором положения Солнца
    XS2 = xS[0]*xS[0] + xS[1]*xS[1] + xS[2]*xS[2];                  // --квадрат модуля вектора положения Солнца
    VECCM = VECC[0]*VECC[0] + VECC[1]*VECC[1] + VECC[2]*VECC[2];    // --квадрат модуля вектора площадей
    CDX = CDX/sqrt(VECCM)/sqrt(XS2);                                // --косинус угла м/у VECC и DXS
    GAM = asin(CDX);                                                // --склонение солнца к плоскости орбиты

    // ----------------------------------------------------------------------------

    vecMul(VECC,xS,VECS);

    VECCS = VECS[0]*VECS[0] +VECS[1]*VECS[1] + VECS[2]*VECS[2];
    VECS[0] = VECS[0]/sqrt(VECCS);
    VECS[1] = VECS[1]/sqrt(VECCS);
    VECS[2] = VECS[2]/sqrt(VECCS);
    VECCN[0] = VECC[0]/sqrt(VECCM);
    VECCN[1] = VECC[1]/sqrt(VECCM);
    VECCN[2] = VECC[2]/sqrt(VECCM);

    vecMul(VECS,VECCN,VECP);

    COSFI = (VECP[0]*x[0] + VECP[1]*x[1] + VECP[2]*x[2])/R;
    SINFI = (VECS[0]*x[0] + VECS[1]*x[1] + VECS[2]*x[2])/R;

    ARGLAT = atan2(SINFI,COSFI);

    IORDER = 3;

    if (lenLightCoef == 9) IORDER = 1;

    KOEF[0]=0.0;
    KOEF[1]=0.0;
    KOEF[2]=0.0;

    for (int i = 0; i < 3; ++i) {
        CGAM = 1.0;                 //cos(gamma)
        for (int k = 0; k < IORDER; ++k) {
            CGAM=CGAM*cos(GAM);
            TERM = CGAM*(A123[i][k]*cos(static_cast<long double>(k+1)*ARGLAT) +
                         B123[i][k]*sin(static_cast<long double>(k+1)*ARGLAT));
            KOEF[i] = KOEF[i] + TERM;
        }
        KOEF[i] = KOEF[i] + A0[i];
        //cout << A0[i] << " ";
    }
    //cout << endl;


    //NU = 1.0; // ----------------------------------------------------------------- не забыть убрать
    BUF = NU * GS_NEW/(ROS*ROS);
    KOEF[0] = KOEF[0]*BUF;
    KOEF[1] = KOEF[1]*BUF;
    KOEF[2] = KOEF[2]*BUF;

    // БЛОК 7.0---Перевод ускорений от РД из НПСК в инерциальную систему координат-

    SP[0] = E1[0]*KOEF[0] + E2[0]*KOEF[1] + E3[0]*KOEF[2];
    SP[1] = E1[1]*KOEF[0] + E2[1]*KOEF[1] + E3[1]*KOEF[2];
    SP[2] = E1[2]*KOEF[0] + E2[2]*KOEF[1] + E3[2]*KOEF[2];

    f[0] += SP[0];
    f[1] += SP[1];
    f[2] += SP[2];

    if (lenLightCoef>0){
        long double cos_gam = cos(GAM);
        long double cos_arglat = cos(ARGLAT);
        long double sin_arglat = sin(ARGLAT);

        DFSL[0][0] = BUF*E1[0]; // dP/dA10 (A_CONST(1))
        DFSL[0][1] = BUF*E1[1]; // dP/dA10 (A_CONST(1))
        DFSL[0][2] = BUF*E1[2]; // dP/dA10 (A_CONST(1))

        DFSL[1][0] = BUF*E2[0]; // dP/dA20 (A_CONST(2))
        DFSL[1][1] = BUF*E2[1]; // dP/dA20 (A_CONST(2))
        DFSL[1][2] = BUF*E2[2]; // dP/dA20 (A_CONST(2))

        DFSL[2][0] = BUF*E3[0]; // dP/dA30 (A_CONST(3))
        DFSL[2][1] = BUF*E3[1]; // dP/dA30 (A_CONST(3))
        DFSL[2][2] = BUF*E3[2]; // dP/dA30 (A_CONST(3))


        DFSL[3][0] = BUF*E1[0]*cos_gam * cos_arglat; // dP/dA11
        DFSL[3][1] = BUF*E1[1]*cos_gam * cos_arglat; // dP/dA11
        DFSL[3][2] = BUF*E1[2]*cos_gam * cos_arglat; // dP/dA11

        DFSL[4][0] = BUF*E2[0]*cos_gam * cos_arglat; // dP/dA21
        DFSL[4][1] = BUF*E2[1]*cos_gam * cos_arglat; // dP/dA21
        DFSL[4][2] = BUF*E2[2]*cos_gam * cos_arglat; // dP/dA21

        DFSL[5][0] = BUF*E3[0]*cos_gam * cos_arglat; // dP/dA31
        DFSL[5][1] = BUF*E3[1]*cos_gam * cos_arglat; // dP/dA31
        DFSL[5][2] = BUF*E3[2]*cos_gam * cos_arglat; // dP/dA31


        DFSL[6][0] = BUF*E1[0]*cos_gam * sin_arglat; // dP/dB11
        DFSL[6][1] = BUF*E1[1]*cos_gam * sin_arglat; // dP/dB11
        DFSL[6][2] = BUF*E1[2]*cos_gam * sin_arglat; // dP/dB11

        DFSL[7][0] = BUF*E2[0]*cos_gam * sin_arglat; // dP/dB21
        DFSL[7][1] = BUF*E2[1]*cos_gam * sin_arglat; // dP/dB21
        DFSL[7][2] = BUF*E2[2]*cos_gam * sin_arglat; // dP/dB21

        DFSL[8][0] = BUF*E3[0]*cos_gam * sin_arglat; // dP/dB31
        DFSL[8][1] = BUF*E3[1]*cos_gam * sin_arglat; // dP/dB31
        DFSL[8][2] = BUF*E3[2]*cos_gam * sin_arglat; // dP/dB31

        if (lenLightCoef>9){
            DFSL[9][0] = BUF*E1[0]*cos(GAM)*cos(GAM)*cos(2.*ARGLAT); // dP/dA12
            DFSL[9][1] = BUF*E1[1]*cos(GAM)*cos(GAM)*cos(2.*ARGLAT); // dP/dA12
            DFSL[9][2] = BUF*E1[2]*cos(GAM)*cos(GAM)*cos(2.*ARGLAT); // dP/dA12

            DFSL[10][0] = BUF*E2[0]*cos(GAM)*cos(GAM)*cos(2.*ARGLAT); // dP/dA22
            DFSL[10][1] = BUF*E2[1]*cos(GAM)*cos(GAM)*cos(2.*ARGLAT); // dP/dA22
            DFSL[10][2] = BUF*E2[2]*cos(GAM)*cos(GAM)*cos(2.*ARGLAT); // dP/dA22

            DFSL[11][0] = BUF*E3[0]*cos(GAM)*cos(GAM)*cos(2.*ARGLAT); // dP/dA32
            DFSL[11][1] = BUF*E3[1]*cos(GAM)*cos(GAM)*cos(2.*ARGLAT); // dP/dA32
            DFSL[11][2] = BUF*E3[2]*cos(GAM)*cos(GAM)*cos(2.*ARGLAT); // dP/dA32

            DFSL[12][0] = BUF*E1[0]*cos(GAM)*cos(GAM)*sin(2.*ARGLAT); // dP/dB12
            DFSL[12][1] = BUF*E1[1]*cos(GAM)*cos(GAM)*sin(2.*ARGLAT); // dP/dB12
            DFSL[12][2] = BUF*E1[2]*cos(GAM)*cos(GAM)*sin(2.*ARGLAT); // dP/dB12

            DFSL[13][0] = BUF*E2[0]*cos(GAM)*cos(GAM)*sin(2.*ARGLAT); // dP/dB22
            DFSL[13][1] = BUF*E2[1]*cos(GAM)*cos(GAM)*sin(2.*ARGLAT); // dP/dB22
            DFSL[13][2] = BUF*E2[2]*cos(GAM)*cos(GAM)*sin(2.*ARGLAT); // dP/dB22

            DFSL[14][0] = BUF*E3[0]*cos(GAM)*cos(GAM)*sin(2.*ARGLAT); // dP/dB32
            DFSL[14][1] = BUF*E3[1]*cos(GAM)*cos(GAM)*sin(2.*ARGLAT); // dP/dB32
            DFSL[14][2] = BUF*E3[2]*cos(GAM)*cos(GAM)*sin(2.*ARGLAT); // dP/dB32

            DFSL[15][0] = BUF*E1[0]*cos(GAM)*cos(GAM)*cos(GAM)*cos(3.*ARGLAT); // dP/dA13
            DFSL[15][1] = BUF*E1[1]*cos(GAM)*cos(GAM)*cos(GAM)*cos(3.*ARGLAT); // dP/dA13
            DFSL[15][2] = BUF*E1[2]*cos(GAM)*cos(GAM)*cos(GAM)*cos(3.*ARGLAT); // dP/dA13

            DFSL[16][0] = BUF*E2[0]*cos(GAM)*cos(GAM)*cos(GAM)*cos(3.*ARGLAT); // dP/dA23
            DFSL[16][1] = BUF*E2[1]*cos(GAM)*cos(GAM)*cos(GAM)*cos(3.*ARGLAT); // dP/dA23
            DFSL[16][2] = BUF*E2[2]*cos(GAM)*cos(GAM)*cos(GAM)*cos(3.*ARGLAT); // dP/dA23

            DFSL[17][0] = BUF*E3[0]*cos(GAM)*cos(GAM)*cos(GAM)*cos(3.*ARGLAT); // dP/dA33
            DFSL[17][1] = BUF*E3[1]*cos(GAM)*cos(GAM)*cos(GAM)*cos(3.*ARGLAT); // dP/dA33
            DFSL[17][2] = BUF*E3[2]*cos(GAM)*cos(GAM)*cos(GAM)*cos(3.*ARGLAT); // dP/dA33

            DFSL[18][0] = BUF*E1[0]*cos(GAM)*cos(GAM)*cos(GAM)*sin(3.*ARGLAT); // dP/dB13
            DFSL[18][1] = BUF*E1[1]*cos(GAM)*cos(GAM)*cos(GAM)*sin(3.*ARGLAT); // dP/dB13
            DFSL[18][2] = BUF*E1[2]*cos(GAM)*cos(GAM)*cos(GAM)*sin(3.*ARGLAT); // dP/dB13

            DFSL[19][0] = BUF*E2[0]*cos(GAM)*cos(GAM)*cos(GAM)*sin(3.*ARGLAT); // dP/dB23
            DFSL[19][1] = BUF*E2[1]*cos(GAM)*cos(GAM)*cos(GAM)*sin(3.*ARGLAT); // dP/dB23
            DFSL[19][2] = BUF*E2[2]*cos(GAM)*cos(GAM)*cos(GAM)*sin(3.*ARGLAT); // dP/dB23

            DFSL[20][0] = BUF*E3[0]*cos(GAM)*cos(GAM)*cos(GAM)*sin(3.*ARGLAT); // dP/dB33
            DFSL[20][1] = BUF*E3[1]*cos(GAM)*cos(GAM)*cos(GAM)*sin(3.*ARGLAT); // dP/dB33
            DFSL[20][2] = BUF*E3[2]*cos(GAM)*cos(GAM)*cos(GAM)*sin(3.*ARGLAT); // dP/dB33
        }
    }

    //очищение
    ER.clear(); EM.clear();
    E1.clear(); E2.clear(); E3.clear();
    VECC.clear(); VECS.clear();  VECCN.clear(); VECP.clear();
}


void Perturbations::vecMul(QVector<long double> a, QVector<long double> b, QVector<long double>&c)
{
    //Можно перенести в модуль MatUtils
    c[0] = a[1]*b[2] - a[2]*b[1];
    c[1] = a[2]*b[0] - a[0]*b[2];
    c[2] = a[0]*b[1] - a[1]*b[0];
}



void Perturbations::setLightCoef(const QVector<long double>& lightCoef)
{
    lenLightCoef = lightCoef.size();
    if (lenLightCoef == 9){
        A0[0] = lightCoef[0];
        A0[1] = lightCoef[1];
        A0[2] = lightCoef[2];

        //в A123 и B123 первый индекс означает координату, вторая порядок
        A123[0][0] = lightCoef[3];
        A123[1][0] = lightCoef[4];
        A123[2][0] = lightCoef[5];

        B123[0][0] = lightCoef[6];
        B123[1][0] = lightCoef[7];
        B123[2][0] = lightCoef[8];

        A123[0][1] = 0.0;
        A123[1][1] = 0.0;
        A123[2][1] = 0.0;

        B123[0][1] = 0.0;
        B123[1][1] = 0.0;
        B123[2][1] = 0.0;

        A123[0][2] = 0.0;
        A123[1][2] = 0.0;
        A123[2][2] = 0.0;

        B123[0][2] = 0.0;
        B123[1][2] = 0.0;
        B123[2][2] = 0.0;
    }

    else if ( lenLightCoef == 21) {
        A0[0] = lightCoef[0];
        A0[1] = lightCoef[1];
        A0[2] = lightCoef[2];

        A123[0][0] = lightCoef[3];
        A123[1][0] = lightCoef[4];
        A123[2][0] = lightCoef[5];

        B123[0][0] = lightCoef[6];
        B123[1][0] = lightCoef[7];
        B123[2][0] = lightCoef[8];

        A123[0][1] = lightCoef[9];
        A123[1][1] = lightCoef[10];
        A123[2][1] = lightCoef[11];

        B123[0][1] = lightCoef[12];
        B123[1][1] = lightCoef[13];
        B123[2][1] = lightCoef[14];

        A123[0][2] = lightCoef[15];
        A123[1][2] = lightCoef[16];
        A123[2][2] = lightCoef[17];

        B123[0][2] = lightCoef[18];
        B123[1][2] = lightCoef[19];
        B123[2][2] = lightCoef[20];
    }
}


//сеттеры
void Perturbations::setBeginEpochTai(QDateTime newEpochTai){
    _beginEpochTai = newEpochTai;
}
void Perturbations::setBeginEpochUtc(QDateTime newEpochUtc){
    _beginEpochUtc = newEpochUtc;
}
void Perturbations::setBeginEpochUtcJd(long double newBeginEpochUtcJd)
{
    _beginEpochUtcJd = newBeginEpochUtcJd;
}
void Perturbations::setBeginEpochTaiJd(long double newBeginEpochTaiJd)
{
    _beginEpochTaiJd = newBeginEpochTaiJd;
}

void Perturbations::setPertNM(int newNM){
    _NM = newNM;
}
void Perturbations::setPertMN(int newMN){
    _MN = newMN;
}
void Perturbations::setPertEarthHarm(bool isEarthHarm){
    _isEarthHarmonics = isEarthHarm;
}
void Perturbations::setPertMoonHarm(bool isMoonHarm){
    _isMoonHarmonics = isMoonHarm;
}
void Perturbations::setPertSun(bool isSun){
    _isSun = isSun;
}
void Perturbations::setPertMoon(bool isMoon){
    _isMoon = isMoon;
}
void Perturbations::setPertPlanet(bool isPlanet){
    _isPlanets = isPlanet;
}
void Perturbations::setPertTides(bool isTides){
    _isTides = isTides;
}
void Perturbations::setPertRadPressure(bool isRadiationPressure){
    _isRadiationPressure = isRadiationPressure;
}
void Perturbations::setPertRelativistic(bool newRelativistic){
    _isRelativistic = newRelativistic;
}

void Perturbations::setLenLightCoef(int value)
{
    lenLightCoef = value;
}

//геттеры

string Perturbations::getFileFond() const
{
    return fondFileName;
}

void Perturbations::setFileFond(const string &value)
{
    fondFileName = value;
}

QString Perturbations::getFileEop() const
{
    return _earthOrientationParams.getFileEop();
}

void Perturbations::setFileEop(const QString &value)
{
    _earthOrientationParams.setFileEop(value);
}


QDateTime Perturbations::getBeginEpochTai() const
{
    return _beginEpochTai;
}

QDateTime Perturbations::getBeginEpochUtc() const
{
    return _beginEpochUtc;
}

int Perturbations::getLenLightCoef() const
{
    return lenLightCoef;
}

int Perturbations::getPertMN(){
    return _MN;
}
int Perturbations::getPertNM(){
    return _NM;
}
bool Perturbations::getPertEarthHarm(){
    return _isEarthHarmonics;
}
bool Perturbations::getPertMoonHarm(){
    return _isMoonHarmonics;
}
bool Perturbations::getPertMoon(){
    return _isMoon;
}
bool Perturbations::getPertSun(){
    return _isSun;
}
bool Perturbations::getPertPlanet(){
    return _isPlanets;
}
bool Perturbations::getPertTides(){
    return _isTides;
}
bool Perturbations::getPertRadPressure(){
    return _isRadiationPressure;
}
bool Perturbations::getPertRelativistic(){
    return _isRelativistic;
}
