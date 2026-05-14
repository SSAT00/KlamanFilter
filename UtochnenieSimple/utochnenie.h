#ifndef UTOCHNENIE_H
#define UTOCHNENIE_H



#include "nm/propagation.h"
#include "nm/DataTypes.h"

#include "Modules.h"

#include <../boost_1_90_0/boost/signals2.hpp>

#include "../toolboxEigen/Eigen/Dense"
using namespace Eigen;


struct satellite{
    Matrix<long double, 6, 1> XE0, XC0; // Массив с стартовыми эталонынми и расчетными параметрами на мерном интервале
    Matrix<long double, 1, 9> LightCoeffs, LightCoeffs_ref; // Массив с коэффициентами СД спутников
    Matrix<long double, Dynamic, 45> Derivatives; // Массив с производными
    Matrix<long double, Dynamic, 45> DerivativesY; // Массив с производными из yout
    std::string name; // Название спутника
    int number; // Hомер спутника
    Matrix<long double, Dynamic, 6> EPH_E, EPH_C, EPH_X; // Массив с эталонными и расчетными эфемеридами
    Matrix<long double, Dynamic, 1> EPH_T_E, EPH_T_C; // Массив с моментами времени в которые получены эфемериды
    Matrix<Matrix<long double, Dynamic, 4>, Dynamic, 1> OBS; // Массив с наблюдениями спутников
    Matrix<Matrix<long double, Dynamic, 4>, Dynamic, 1> OBS_STAN; // Масссив наблюдениями станций
    Matrix<int, Dynamic, Dynamic> UsageObs; // Массив с флагами включения наблюдений
    Matrix<int, Dynamic, Dynamic> UsageObs_STAN; // Массив с флагами включения наблюдений станций
    Matrix<int, Dynamic, 1> Cyclogram; // Массив циклограммы спутника (по условию задачи)
    Matrix<int, Dynamic, 1> CyclogramStan; // Массив Циклограммы спутника на назменые станции (по условию задачи)
    long double GMIN, GMAX; // Углы раствора антенн
    Matrix<long double, Dynamic, Dynamic> Q;
    Matrix<long double, Dynamic, 1> G;
    long double dempf;
    QVector<long double> mists;
    Matrix<long double, Dynamic, 1> IdentedSysErr;
};

struct stantion{
    long double X, Y, Z;
    double GMIN, GMAX;
    Matrix<long double, Dynamic, 3> EPH_E;
    Matrix<long double, Dynamic, 1> EPH_T_E;
};


class Utochnenie
{
private:
    int NSAT; // Число спутников
    QVector<satellite> satellites;

    int NSTAN;
    QVector<stantion> stantions;

    long double sclx, sclv, scllc;
    int NPAR;

    long double DT, BLOCK, GAP, SEANS, INTERVAL; // Параметры временной сетки
    int NSES;
    int NM;

    int GeoNM, GeoMN;
    bool pertTides, pertSun, pertMoon, pertPlanets, pertRadiation, pertRelatEffects;

    QDateTime dt_p, dt_s, dt_f;

    std::vector<std::mt19937> satelliteGenerators;

    Matrix<long double, Dynamic, Dynamic> SysErrSat, SysErrStan;

    long double NoiseSat, NoiseStan, SysErr;

    long double dempf, acc;

    string cleanLine(string);
    long double QDate2Julian(QDateTime);
    long double Sid2000(long double);

    bool fl_stop;
    int max_iters;

public:
    Utochnenie();


    boost::signals2::signal<void(int, int)> sig_setNIters;
    boost::signals2::signal<void()> sig_clearNIters;
    boost::signals2::signal<void(double, int)> sig_setMean;
    boost::signals2::signal<void()> sig_clearMean;
    boost::signals2::signal<void(double, int)> sig_setSTD;
    boost::signals2::signal<void()> sig_clearSTD;
    boost::signals2::signal<void(double, int)> sig_setCond;
    boost::signals2::signal<void()> sig_clearCond;
    boost::signals2::signal<void(double, int)> sig_setDqX;
    boost::signals2::signal<void()> sig_clearDqX;
    boost::signals2::signal<void(double, int)> sig_setDqSD;
    boost::signals2::signal<void()> sig_clearDqSD;
    boost::signals2::signal<void(int, int)> sig_setNObs;
    boost::signals2::signal<void()> sig_clearNObs;
    boost::signals2::signal<void(int, QString, QString)> sig_setLocalProgress;
    boost::signals2::signal<void(int, QString)> sig_setProgress;

    void SetNSAT(int);
    void SetNSTAN(int);
    void SetSatCoords(Matrix<long double, Dynamic, 6>);
    void SetStanCoords(Matrix<long double, Dynamic, 3>);
    void SetSatNames(Matrix<QString, Dynamic, 1>);
    void SetLPC(Matrix<long double, Dynamic, 9>);
    void SetSatAngle(Matrix<long double, Dynamic, 2>);
    void SetStanAngle(Matrix<long double, Dynamic, 2>);
    void SetCyclogramSatSat(Matrix<int, Dynamic, Dynamic>);
    void SetCyclogramSatStan(Matrix<int, Dynamic, Dynamic>);

    void SetTimeNet(long double, long double, long double, long double);
    void SetNavigationFrames(QDateTime, QDateTime);
    void SetPerturb(int, int, bool, bool, bool, bool, bool, bool);
    void SetNParamsRefine(int);

    void SetNoises(long double, long double, long double);
    void SetDempf(long double);
    void SetAcc(long double);

    input_data_propagation_t GetInputDataProp(QDateTime, QDateTime, int, bool);
    Propagation GetPropagationObj();
    QString GetSatName(int i){return QString::fromStdString(satellites[i].name);};

    void PropagateRef(int);
    void PropagateDef(int);
    void PropagateFirst();
    void PropagateStantion();
    void Propagate();

    void ObservateSat(int);
    void Observate();
    void IdentSysErr(int);
    bool ObsInBlock(int, int);

    void RefineSat(int);
    void Refine();

    void Moving();
    void MoveSat(int);

    int SatellitesVisible(Matrix<double, 3, 1>, Matrix<double, 3, 1>, int);
    int StantionsVisible(Matrix<double, 3, 1>, Matrix<double, 3, 1>, int);

    void NormalizeQG(int, Matrix<long double, Dynamic, Dynamic>&, Matrix<long double, Dynamic, 1>&);
    void UnNormalizeDq(int, Matrix<long double, Dynamic, 1>&, Matrix<long double, Dynamic, Dynamic>&, Matrix<long double, Dynamic, 1>&);
    void RecountDempfSat(int);

    void WriteSTWErrs(int);
    void WriteSatellitesOBS(int);
    void WriteStartParams();
    void WriteSatDempf(int);

    void ClearResultsFolder();

    void RunApp();

    void stop();
};






































#endif // UTOCHNENIE_H
