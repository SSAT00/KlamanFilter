#ifndef MODULES_H
#define MODULES_H


#include <QObject>
#include <QCoreApplication>
#include <QVector>
#include <QDebug>
#include <QString>
#include <QDir>
#include <QStringList>

#include <QCheckBox>
#include <QFrame>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QPainter>
#include <QTextBrowser>

#include "string"
#include <iostream>
#include <fstream>
#include "iomanip"
#include "thread"
#include "random"
#include "algorithm"
#include <chrono>
#include <cmath>

#include "../toolboxEigen/Eigen/Dense"


using namespace Eigen;
using namespace std;

double standardDeviation2(QVector<double> data, double& mean);
Matrix<int, Dynamic, Dynamic> GetCyclogramSat();
Matrix<int, Dynamic, Dynamic> GetCyclogramStan();
typedef Eigen::Matrix<long double, 6, 1> Vector6ld;
typedef Eigen::Matrix<long double, 3, 1> Vector3ld;
typedef Eigen::Matrix<long double, 3, 3> Matrix3ld;
Vector3ld error_to_stw(const Vector6ld&, const Vector6ld&);
long double getConditionNumberSVD(const Eigen::Matrix<long double, Eigen::Dynamic, Eigen::Dynamic>&);


struct ChoosedSatellite{
    QString Name;
    QString X, Y, Z, VX, VY, VZ;
    QString A01, A02, A03, A11, A12, A13, B11, B12, B13;
    QString Gmin, Gmax;
    int NGROUP;
};

struct ChoosedGroup{
    QString Name;
    int N;
};

struct ChoosedStantion{
    QString PHI, LAM, H;
    QString Gmin, Gmax;
};

#endif // MODULES_H




















































