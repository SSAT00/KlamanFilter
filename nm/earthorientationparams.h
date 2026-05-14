#ifndef EARTHORIENTATIONPARAMS_H
#define EARTHORIENTATIONPARAMS_H
#include <QVector>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <fstream>
#include <iostream>

#include "DataTypes.h"
#include "timesystem.h"

class EarthOrientationParams
{
private:

    const QDate     _date010192 = QDate(1992, 1, 1);
    const QDateTime _date010120 = QDateTime(QDate(2000, 1, 1), QTime(0, 0, 0));
    const QDateTime _J2000 = QDateTime(QDate(2000, 1, 1), QTime(12, 0, 0));
    const long double _arcsec2Rad = 4.848136811095359935899141E-6; // перевод из секунд дуги в радианы
    const int _interpOrder = 12;                                   // Порядок интерполяции Лагранжа


    // границы времени рабочего массива listEop
    QDateTime begEpochUtc;
    QDateTime endEpochUtc;



    // рабочий массив координат полюса считанный из файла
    QVector<eop_t> listEop;
    QString fileName; // путь к файлу для чтения ПВЗ

    bool outRange = false;// признак выхода за пределы данных ПВЗ


public:
    EarthOrientationParams();

    //метод заполняющий listEop
    void getListEop(QDateTime begTimeUtc,QDateTime endTimeUtc);

    //метод возвращающий конкретное синтерполированное значение ПВЗ на задаваемый момент времени, который лежит в пределах begEpochUtc и endEpochUtc
    void getEop(QDateTime curTimeUtc, eop_t &eop);
    // метод интерполяции
    eop_t interpolateEOP(QDateTime epochUTC);
    //переход ко времени ГЛОНАСС
    long double calcTime(QDateTime epochUTC);

    long double interpolation(const QVector<long double> & X, const QVector<long double> & Y, const long double x);

    QString getFileEop() const;
    void setFileEop(const QString &value);

};

#endif // EARTHORIENTATIONPARAMS_H
