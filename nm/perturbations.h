#ifndef PERTURBATIONS_H
#define PERTURBATIONS_H

#include <cmath> //для работы с long double
#include <QDateTime>
#include <QVector>
#include <fstream>
#include <iostream>
#include <QString>
#include <quadmath.h>

#include "initconst.h"
#include "sofa/sofa.h"
#include "nm/timesystem.h"
#include "nm/earthorientationparams.h"
#include "matutils.h"

using namespace std;

class Perturbations {

private:
    // Определение типов данных для массивов
    using vec3 = array<double, 3>;
    using mat3x3 = array<array<double, 3>, 3>;
    using vec6 = array<double, 6>;

    // матрица поворота (вычисляется в отдельном классе)


    // Флаги возмущений
    int _NM = 0 , _MN = 0;
    bool _isEarthHarmonics    = false;
    bool _isMoonHarmonics     = false;
    bool _isMoon              = false;
    bool _isSun               = false;
    bool _isPlanets           = false;
    bool _isTides             = false;
    bool _isRadiationPressure = false;
    bool _isRelativistic      = false;

    string fondFileName;

    //Константы
    const long double _GE = InitConst::gravParam[EARTH];      // гравитационный параметрв для Земли
    const long double _GS = InitConst::gravParam[SUN];        // гравитационный параметрв для Солнца
    const long double _GM = InitConst::gravParam[MOON];       // гравитационный параметр для Луны
    const long double _RE = InitConst::raduisEarth;           // экваториальный радиус Земли

    //гравитационные параметры планет
    const long double _GPLS[9] = {InitConst::gravParam[1],    //Меркурий
                                  InitConst::gravParam[2],    //Венера
                                  InitConst::gravParam[3],    //Земля
                                  InitConst::gravParam[4],    //Марс
                                  InitConst::gravParam[5],    //Юпитер
                                  InitConst::gravParam[6],    //Сатурн
                                  InitConst::gravParam[7],    //Уран
                                  InitConst::gravParam[8],    //Нептун
                                  InitConst::gravParam[9]};   //Плутон



    long double CV[2][25][25];                           //заполняются в InitConst::GeoConst
    long double Q[23][23], QX[23][23], QZ[23][23];       //заполняются в InitConst::GeoConst
    long double CNST[23][24], SNST[23][24];
    long double _gNorm = InitConst::gNorm;

    int lenLightCoef;
    long double A0[3],A123[3][3],B123[3][3];

    QDateTime _beginEpochTai;     // начальная эпоха вектора состояния КА в TAI
    QDateTime _beginEpochUtc;     // начальная эпоха вектора состояния КА в UTC
    long double _beginEpochTaiJd; // начальная эпоха вектора состояния КА в TAI в юлианских днях
    long double _beginEpochUtcJd; // начальная эпоха вектора состояния КА в UTC в юлианских днях


public:
    Perturbations();

    EarthOrientationParams _earthOrientationParams;

    void init();
    void forse(int nxy, QVector<long double> x, QVector<long double> y, long double t, QVector<long double>& f);

    // третье тело
    void gravThirdBody(long double mu, QVector<long double> x,
                                       QVector<long double> xs, bool pert,
                                       QVector<long double>& f,
                                       QVector<QVector<long double>>& f2);

    //геопотенциал
    void geopot(long double t, QVector<long double> x,
                               QVector<long double> &f,
                               QVector<QVector<long double>>& f2);
    void cangem(QVector<long double> x,
                QVector<QVector<long double>> AR,
                QVector<long double> &CONX,
                QVector<QVector<long double>>& CONX2);
    void initGeoConst();
    void initEop(QDateTime beginEpochUtc, QDateTime endEpochUtc);
    void changeGarmCoef(long double tjd);
    void tides(QVector<long double> x,QVector<long double> xMoon, QVector<long double> xSun,QVector<long double>& f);

    //чтение фонда
    void read438_16(long double t, QVector<long double>& xM, QVector<long double>& xS, QVector<QVector<long double>>& xPLS);
    void coor438_16(int i, __float128 t, QVector<__float128> buf16,  QVector<__float128>& xc);
    void cheb_16(__float128 a, __float128 b, __float128 t, int ist, QVector<__float128>& tc, QVector<__float128>& tcp);


    void read438(long double t, QVector<long double>  &xM, QVector<long double> &xS, QVector<QVector<long double>> &xPLS);
    void coor438(int i, long double t, QVector<long double> buf16,  QVector<long double> &xc);
    void cheb(long double a, long double b, long double t, int ist, QVector<long double> &tc, QVector<long double> &tcp);

    //Релятивизм
    void rltvt(QVector<long double> x, QVector<long double> v, QVector<long double> xS, QVector<long double> &f);

    //Радиационное давление
    void radPressure2(QVector<long double> x,  QVector<long double> v, //координаты и скорости
                      QVector<long double> xS, QVector<long double> xM,//координаты Солнца и Луны
                      QVector<long double> &f,                         //ускорения
                      QVector<QVector<long double>>& DFSL,             //частные производные
                      QVector<QVector<long double>>& f2);              //вторые производные сил

    void radPressure(QVector<long double> x,  QVector<long double> v,
                     QVector<long double> xS, QVector<long double> xM, QVector<long double> &f,QVector<QVector<long double>>& DFSL);

    void vecMul(QVector<long double> a, QVector<long double>b, QVector<long double>& c);


    // сеттеры на время
    void setBeginEpochTai(QDateTime newEpochTai);
    void setBeginEpochUtc(QDateTime newEpochUtc);
    void setBeginEpochTaiJd(long double newBeginEpochTaiJd);
    void setBeginEpochUtcJd(long double newBeginEpochUtcJd);

    //сеттер на коэф-ты светового давления
    void setLightCoef(const QVector<long double>& lightCoef);

    //сеттеры на структуру возмущения
    void setPertNM(int newNM);
    void setPertMN(int newMN);
    void setPertEarthHarm(bool isEarthHarm);
    void setPertMoonHarm(bool isMoonHarm);
    void setPertMoon(bool isMoon);
    void setPertSun(bool isSun);
    void setPertPlanet(bool isPlanet);
    void setPertTides(bool isTides);
    void setPertRadPressure(bool isRadiationPressure);
    void setPertRelativistic(bool newRelativistic);

    //геттеры на время
    QDateTime getBeginEpochTai() const;
    QDateTime getBeginEpochUtc() const;

    // геттеры световые коэф-ты
    int getLenLightCoef() const;
    void setLenLightCoef(int value);

    //геттеры на структуру возмущений
    int getPertNM();
    int getPertMN();
    bool getPertEarthHarm();
    bool getPertMoonHarm();
    bool getPertMoon();
    bool getPertSun();
    bool getPertPlanet();
    bool getPertTides();
    bool getPertRadPressure();
    bool getPertRelativistic();

    // файл фонда
    string getFileFond() const;
    void setFileFond(const string &value);

    //файл ПВЗ
    QString getFileEop() const;
    void setFileEop(const QString &value);

};

#endif // PERTURBATIONS_H
