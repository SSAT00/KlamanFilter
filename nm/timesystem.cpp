#include "timesystem.h"

TimeSystem::TimeSystem()
{

}

int TimeSystem::leapSeconds(int year, int month){
    // Список всех високосных секунд и соответствующего смещения TAI-UTC.
    // Список должен поддерживаться в актуальном состоянии вручную или через внешний источник.
    // Даты указаны как даты ВСТУПЛЕНИЯ в силу нового смещения (00:00:00 UTC указанной даты).
    const QVector<LeapSecondEntry> leapSecondsTable = {
        {QDate(1972, 1, 1), 10},
        {QDate(1972, 7, 1), 11},
        {QDate(1973, 1, 1), 12},
        {QDate(1974, 1, 1), 13},
        {QDate(1975, 1, 1), 14},
        {QDate(1976, 1, 1), 15},
        {QDate(1977, 1, 1), 16},
        {QDate(1978, 1, 1), 17},
        {QDate(1979, 1, 1), 18},
        {QDate(1980, 7, 1), 19},
        {QDate(1981, 7, 1), 20},
        {QDate(1982, 7, 1), 21},
        {QDate(1983, 7, 1), 22},
        {QDate(1985, 7, 1), 23},
        {QDate(1988, 1, 1), 24},
        {QDate(1990, 1, 1), 25},
        {QDate(1991, 1, 1), 26},
        {QDate(1992, 7, 1), 27},
        {QDate(1993, 7, 1), 28},
        {QDate(1994, 7, 1), 29},
        {QDate(1996, 1, 1), 30},
        {QDate(1997, 7, 1), 31},
        {QDate(1999, 1, 1), 32},
        {QDate(2006, 1, 1), 33},
        {QDate(2009, 1, 1), 34},
        {QDate(2012, 7, 1), 35},
        {QDate(2015, 7, 1), 36},
        {QDate(2017, 1, 1), 37} // Последняя добавленная секунда
    };

    QDate targetDate(year, month, 1); // Используем 1-е число месяца для сравнения

    // Если дата некорректна (например, 2025 год), возвращаем последнее известное значение
    if (!targetDate.isValid()) {
        qWarning() << "Передана некорректная дата:" << year << month;
        return leapSecondsTable.last().offsetSeconds;
    }

    int currentOffset = 10; // Начальное смещение с 1972 года

    // Ищем в таблице первое вхождение даты, которая больше нашей целевой даты.
    // Это означает, что предыдущая запись содержит правильное смещение.
    for (const auto& entry : leapSecondsTable) {
        if (entry.date > targetDate) {
            // Прерываем цикл и возвращаем значение из ПРЕДЫДУЩЕЙ записи.
            break;
        }
        currentOffset = entry.offsetSeconds;
    }

    return currentOffset;
}
