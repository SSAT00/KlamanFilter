#ifndef INTEGRATOR_H
#define INTEGRATOR_H
#include <cmath>    //используется для работы с long double

#include "nm/perturbations.h"
#include "nm/DataTypes.h"

using namespace std;

class Integrator {

private:

    //Входные параметры
    int IMODE = 1 ; //Режим работы

    int nxy ;  //кол-во интегрируемых уравнений второго порядка

    int ns;    //Кол-во узловых точек на шаге
    int ni;    //Кол-во итераций
    long double etolErr; //Локальная задаваемая точность
    long double step ;   //Шаг интегрирования

    //выходные параметры
    int nst;   //Кол-во обращений к правым частям
    int ncf;   //Кол-во итераций для решения нелинейных уравнений

    QVector<long double> x,y;          //Начальные условия: интегрируемое значение и первая производная

    QVector<QVector<long double>> xGrid,yGrid; //Массивы интегрируемых значений {кол-во моментов * кол-во уравнений}

public:
    Integrator();
    ~Integrator();

    Perturbations perturbations;  // класс подключаемых возмущений

    QVector<ephemeris_t> ephemeris;  // посчитанные эфемериды на заданной сетке

    void init(int imode, int nxy, int ns, long double etol, long double step,QVector<long double> x, QVector<long double> y);
    void lobbie(QVector<long double>);  // коллокационный интегратор Lobbie
    void output_forecast(QVector<long double> timeGrid,
                         long double t, long double h,long double h2,
                         QVector<long double> c,
                         QVector<QVector<long double>> a_,
                         QVector<long double> x,
                         QVector<long double> y,
                         int& IOBS);

    //Массив узловых значений на шаге
    static const long double LobbieSpacing[];

    // сеттеры
    void setIMODE(int value);
    void setNxy(int value);
    void setNs(int value);
    void setNi(int value);
    void setEtolErr(long double value);
    void setStep(long double value);

    //сеттер параметров
    void setX(const QVector<long double> &value);
    void setY(const QVector<long double> &value);

    // геттеры
    QVector<ephemeris_t> getEphemeris() const;

    QVector<long double> getX() const;
    QVector<long double> getY() const;

    int getIMODE() const;

    int getNxy() const;
    int getNs() const;
    int getNi() const;
    long double getEtolErr() const;
    long double getStep() const;
    int getNst() const;
    int getNcf() const;

    void integratorClear();   // очищение НУ, эфемерид и массивов интегрируемых значений
    void ephemerisClear();    // очищение эфемерид
    void xyClear();           // очищение начальных условий
    void xyGridClear();       // очищение накопленных массивов


    QVector<QVector<long double> > getXGrid() const;
    QVector<QVector<long double> > getYGrid() const;

    void setXGrid(const QVector<QVector<long double> > &value);
    void setYGrid(const QVector<QVector<long double> > &value);
};

#endif // INTEGRATOR_H
