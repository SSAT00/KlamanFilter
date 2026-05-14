#ifndef DBWORKER_H
#define DBWORKER_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

#include<QVector>
#include<QString>

struct PackageSatelliteConstellation{
    int N;
    QString Name;
    QString Description;
    int YY, MM, DD, h, m, s;
};

struct PackageStantionConstellation{
    int N;
    QString Name;
};

struct PackageSatelliteGroup{
    int N;
    int NConstellation;
    QString Name;
};

struct PackageSatellites{
    int NGROUP;
    QString SatName;
    int SatNumberInGroup;
    QString X, Y, Z, VX, VY, VZ;
    QString A01, A02, A03, A11, A12, A13, B11, B12, B13;
    QString Gmin, Gmax;
};

struct PackageStantions{
    int NCONST;
    QString PHI, LAM, H, GMIN, GMAX;
    QString Description;
};

class DBWorker
{
private:
    QSqlDatabase db;
    QVector<PackageSatelliteConstellation> Constellations;
    QVector<PackageSatelliteGroup> Groups;
    QVector<PackageSatellites> Satellites;

    QVector<PackageStantionConstellation> ConstellationsSTAN;
    QVector<PackageStantions> Stantions;

public:
    DBWorker();
    ~DBWorker();
    void ConnectToDb();
    void ReadConstellationsSAT();
    void ReadGroups();
    void ReadSatellites();
    void ReadConstellationsSTAN();
    void ReadStantions();

    QVector<PackageSatelliteConstellation> GetConstellationsSAT();
    QVector<PackageSatelliteGroup> GetGroups();
    QVector<PackageSatellites> GetSatellites();

    QVector<PackageStantionConstellation> GetConstelationsSTAN();
    QVector<PackageStantions> GetStantions();
};

#endif // DBWORKER_H




































