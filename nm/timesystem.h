#ifndef TIMESYSTEM_H
#define TIMESYSTEM_H
#include <QDateTime>
#include <QDebug>

struct LeapSecondEntry {
    QDate date;
    int offsetSeconds;
};

class TimeSystem
{
private:


public:
    TimeSystem();


    static int leapSeconds(int year, int month);

};

#endif // TIMESYSTEM_H
