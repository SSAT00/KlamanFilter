#ifndef DATATYPES_H
#define DATATYPES_H
#include <QDateTime>


struct ephemeris_t {
       QDateTime epoch;
       long double pos[6];
       long double time;
       QVector<long double> derivatives_x, derivatives_y;
   };

struct eop_t {
    QDateTime epoch;
    long double mjd;
    long double xp;   //координаты
    long double yp;   //полюса
    long double dUt1; //рассхождение шкал
    long double epochTai;

};

struct isl_t {
    // Набор измерений включает в себя
    // измерения одного приемника с несколькими передатчиками
    // каждое измерение имеет: время привязки, псевдодальность, псевдоскорость
    int numPrd;         // Номер передатчика (1-24: системный номер)
    QDateTime t;        // Время привязки измерения: должно быть точным
    long double dist;   // Псевдодальность = c*time_delay
    long double vels;   // Псевдоскорость = -dopler/freq*c (freq - частота сигнала передачи)
};

struct clk_t{
    QDateTime epoch; // эпоха ЧВП
    long double t;   // время привязки ЧВП
    long double tau; // ЧВП
};

// Структура входных данных для модуля прогноза
struct input_data_propagation_t {
    // Параметры временной сетки
    QDateTime ts;     //эпоха начала прогноза
    QDateTime tf;     //эпоха конца прогноза
    long double step; //шаг выдачи

    QDateTime t0;                       //эпоха начальных параметров
    QVector<long double> x;             // координаты
    QVector<long double> y;             // скорости
    QVector<long double> lightCoef;     // параметры светового давления

    int integrOrder = 12;               // порядок интегратора
    long double integrEtol = 1e-10;        // порядок задаваемой точности
    long double integrStep = 100.0;    // постоянный шаг интегрирования

    int GeoNM = 0 ;                     // порядок гармоник геопотенциала
    int GeoMN = 0 ;                     // степень гармоник геопотенциала
    bool pertMoon = 0;                  // возмущение от Луны
    bool pertSun = 0;                   // возмущение от Солнца
    bool pertPlanets = 0 ;              // возмущение от планет
    bool pertRadiation = 0;             // возмущение от РД
    bool pertRelatEffects = 0;          // возмущение от релятив. эффектов
    bool pertTides = 0;                 // возмущение от твердых приливов

};

struct input_data_determination_sp3_t {


    QVector<ephemeris_t> ephSp3;        // эфемериды Sp3 по которым надо уточнить
                                        // должны быть в ICRS

    QDateTime t0;                       // Эпоха начальных параметров
    QVector<long double> x;             // Начальные параметры: координаты
    QVector<long double> y;             // Начальные параметры: скорости
    QVector<long double> lightCoef;     // Начальные коэф-ты СД

    int integrOrder = 12;               // порядок интегратора
    long double integrEtol = 1e-10;     // порядок задаваемой точности
    long double integrStep = 100.0;     // постоянный шаг интегрирования

    int GeoNM = 0 ;                     // порядок гармоник геопотенциала
    int GeoMN = 0 ;                     // степень гармоник геопотенциала
    bool pertMoon = 0;                  // возмущение от Луны
    bool pertSun = 0;                   // возмущение от Солнца
    bool pertPlanets = 0 ;              // возмущение от планет
    bool pertRadiation = 0;             // возмущение от РД
    bool pertRelatEffects = 0;          // возмущение от релятив. эффектов
    bool pertTides = 0;                 // возмущение от твердых приливов

};

struct input_data_determination_isl_t {

    QVector<isl_t> isl;                 // набор измерений МСИ
    QVector<QVector<ephemeris_t>> ephPrd; // набор эфемерид передатчиков
                                        // должны быть в ICRS

    QDateTime t0;                       // Эпоха начальных параметров
    QVector<long double> x;             // Начальные параметры: координаты
    QVector<long double> y;             // Начальные параметры: скорости
    QVector<long double> lightCoef;     // Начальные коэф-ты СД

    int integrOrder = 12;               // порядок интегратора
    long double integrEtol = 1e-10;     // порядок задаваемой точности
    long double integrStep = 100.0;     // постоянный шаг интегрирования

    int GeoNM = 0 ;                     // порядок гармоник геопотенциала
    int GeoMN = 0 ;                     // степень гармоник геопотенциала
    bool pertMoon = 0;                  // возмущение от Луны
    bool pertSun = 0;                   // возмущение от Солнца
    bool pertPlanets = 0 ;              // возмущение от планет
    bool pertRadiation = 0;             // возмущение от РД
    bool pertRelatEffects = 0;          // возмущение от релятив. эффектов
    bool pertTides = 0;                 // возмущение от твердых приливов

};




#endif // DATATYPES_H
