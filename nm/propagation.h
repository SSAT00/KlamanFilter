#ifndef PROPAGATION_H
#define PROPAGATION_H
#include <QtMath>

#include "DataTypes.h"
#include "integrator.h"
#include "timesystem.h"

class Propagation {

private:
    //входные параметры временной сетки (utc)
    QDateTime _beginEpoch;              // начальная эпоха вектора состояния КА
    QDateTime _beginForecast;           // начало интервала прогнозирования
    QDateTime _endForecast;             // конец интервала прогнозирования
    long double step;                   // шаг выдачи

    // параметры временной сетки TAI
    QDateTime _beginEpochTai;
    QDateTime _beginForecastTai;
    QDateTime _endForecastTai;
    // минимальаня и максимальная эпоха на временной сетке
    QDateTime _minEpochTai, _maxEpochTai;
    QDateTime _minEpochUtc, _maxEpochUtc;


    QVector<long double> _gridTime;      // временная сетка (для интегрирования вперёд и/или назад) в секундах
    QVector<long double> _gridTime0;     // временная сетка для интегрирования, инициализируется
    QVector<QDateTime> _gridTimeUTC;     // временная сетка дат
    QVector<ephemeris_t> _ephemeris;     // вычисленные эфемериды

    QVector<long double> x0, y0;            // начальные условия для старта интегрирования на эпоху _beginEpoch

    QVector<QVector<long double>> xGrid, yGrid; //интегрируемые значения эфемерид + частные производные

    Integrator _integrator;             // интегратор

    void fillGridTime(QDateTime beginEpoch, QDateTime  beginForecast, QDateTime endForecast, long double step, QVector<long double>& gridTime);

    QString fileEop;
    string fileFond;

public:
    Propagation();

    void init(input_data_propagation_t);               //Инициализация прогнозирования движения, задание НУ
    void calculate();                                  //Запуск прогноза
    QVector<ephemeris_t> getEphemeris() const;         //Получение эфемерид

    void initImprove();                                //Инициализация метода уточнения, задание НУ для уточнения
    void calculateImprove();                           //Запуск интегрирования уравнений с частными производными
    QVector<QVector<long double>> getImprove() const;  //Получение эфемерид и матрицы частных производных





    QVector<long double> getX() const;
    QVector<long double> getY() const;

    void propagationClear();

    string getFileFond() const;
    QString getFileEop() const;

    void setFileFond(const string &value);
    void setFileEop(const QString &value);

    //Н.У. для интегрирования
    QVector<long double> getX0() const;
    QVector<long double> getY0() const;

    void setX0(const QVector<long double> &value);
    void setY0(const QVector<long double> &value);
};

#endif // PROPAGATION_H
