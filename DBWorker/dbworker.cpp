#include "dbworker.h"

DBWorker::DBWorker()
{
    ConnectToDb();
    ReadConstellationsSAT();
    ReadConstellationsSTAN();
    ReadGroups();
    ReadSatellites();
    ReadStantions();
}

void DBWorker::ConnectToDb() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("DBWorker/DB/DataBaseSatellitesStantions.db");
    if (!db.open()) {
        qDebug() << "Ошибка подключения:" << db.lastError().text();
    } else {
        qDebug() << "База данных успешно открыта!";
    }
}

void DBWorker::ReadConstellationsSAT(){
    QSqlQuery query;
    if (query.exec("SELECT №, Name, YY, MM, DD, h, m, s, Description FROM SatelliteConstellation")) {
        while (query.next()) {
            PackageSatelliteConstellation pack;
            pack.N = query.value(0).toInt();
            pack.Name = query.value(1).toString();
            pack.YY = query.value(2).toInt();
            pack.MM = query.value(3).toInt();
            pack.DD = query.value(4).toInt();
            pack.h = query.value(5).toInt();
            pack.m = query.value(6).toInt();
            pack.s = query.value(7).toInt();
            pack.Description = query.value(8).toString();
            Constellations.push_back(pack);
        }
    }
}

void DBWorker::ReadGroups(){
    QSqlQuery query;
    if (query.exec("SELECT №, №constellation, Name FROM SatelliteGroupName")) {
        while (query.next()) {
            PackageSatelliteGroup pack;
            pack.N = query.value(0).toInt();
            pack.NConstellation = query.value(1).toInt();
            pack.Name = query.value(2).toString();
            Groups.push_back(pack);
        }
    }
}

void DBWorker::ReadSatellites(){
    QSqlQuery query;
    query.prepare("SELECT Name, №group, №satingroup, X, Y, Z, VX, VY, VZ, A01, A02, A03, A11, A12, A13, B11, B12, B13, Gmin, Gmax FROM Satellites");
    if (query.exec()) {
        while (query.next()) {
            PackageSatellites pack;
            pack.SatName = query.value(0).toString();
            pack.NGROUP = query.value(1).toInt();
            pack.SatNumberInGroup = query.value(2).toInt();
            pack.X = query.value(3).toString();
            pack.Y = query.value(4).toString();
            pack.Z = query.value(5).toString();
            pack.VX = query.value(6).toString();
            pack.VY = query.value(7).toString();
            pack.VZ = query.value(8).toString();
            pack.A01 = query.value(9).toString();
            pack.A02 = query.value(10).toString();
            pack.A03 = query.value(11).toString();
            pack.A11 = query.value(12).toString();
            pack.A12 = query.value(13).toString();
            pack.A13 = query.value(14).toString();
            pack.B11 = query.value(15).toString();
            pack.B12 = query.value(16).toString();
            pack.B13 = query.value(17).toString();
            pack.Gmin = query.value(18).toString();
            pack.Gmax = query.value(19).toString();
            Satellites.push_back(pack);
        }
    }
}

void DBWorker::ReadConstellationsSTAN(){
    QSqlQuery query;
    if (query.exec("SELECT №, Name FROM StantionConstellation")) {
        while (query.next()) {
            PackageStantionConstellation pack;
            pack.N = query.value(0).toInt();
            pack.Name = query.value(1).toString();
            ConstellationsSTAN.push_back(pack);
        }
    }
}

void DBWorker::ReadStantions(){
    QSqlQuery query;
    query.prepare("SELECT №constellation, Description, PHI, LAM, H, GMIN, GMAX FROM Stantions");
    if (query.exec()) {
    while (query.next()) {
        PackageStantions pack;
        pack.NCONST = query.value(0).toInt();
        pack.Description = query.value(1).toString();
        pack.PHI = query.value(2).toString();
        pack.LAM = query.value(3).toString();
        pack.H = query.value(4).toString();
        pack.GMIN = query.value(5).toString();
        pack.GMAX = query.value(6).toString();
        Stantions.push_back(pack);
        }
    }
}

QVector<PackageSatelliteConstellation> DBWorker::GetConstellationsSAT(){
    return Constellations;
}

QVector<PackageSatelliteGroup> DBWorker::GetGroups(){
    return Groups;
}

QVector<PackageSatellites> DBWorker::GetSatellites(){
    return Satellites;
}

QVector<PackageStantionConstellation> DBWorker::GetConstelationsSTAN(){
    return ConstellationsSTAN;
}

QVector<PackageStantions> DBWorker::GetStantions(){
    return Stantions;
}

































