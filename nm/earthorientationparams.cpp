#include "earthorientationparams.h"


EarthOrientationParams::EarthOrientationParams()
{
    
}

void EarthOrientationParams::getListEop(QDateTime begTimeUtc, QDateTime endTimeUtc)
{
    //процедура считывающая файл с координатами полюса и расхожением шкал времени
    //заполняет в поле класса массив listEop

    QFile file(fileName);
    QTextStream in(&file);
    QString line;
    QStringList tokens;

    eop_t eop, eopFirst, eopLast;

    int year, month, day;

    //начальная эпоха должна быть меньше конечной
    QDateTime begTimeUtc0 = begTimeUtc.addDays(-static_cast<int>(_interpOrder / 2)); //левая граница
    QDateTime endTimeUtc0 = endTimeUtc.addDays(_interpOrder / 2 - 1);  //правая граница

    if (begTimeUtc.date() == endTimeUtc.date()){
        //запрашивается интервал в пределах одних суток
        endTimeUtc0 = endTimeUtc0.addDays(1);
    }

    begTimeUtc0.setTime(QTime(0, 0, 0));
    endTimeUtc0.setTime(QTime(0, 0, 0));

    if (listEop.size()!=0) {
        listEop.clear();   //если были ранее какие-то данные то всё стереть
        outRange = false;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error reading file:" << fileName;

        return; // Прерываем выполнение
        }
        int countStr=0;

        // Пропускаем первые 14 строк (заголовок)
        while (countStr < 14 && !in.atEnd()) {
            line = in.readLine();
            countStr++;
        }
        countStr = 0;
        while ((line = in.readLine()).isEmpty() == false){

            if (line.trimmed().isEmpty()) continue; // Пропускаем пустые строки

            // Разбиваем строку на токены
            tokens = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);

            // Проверяем, что строка содержит данные
            if (tokens.size() < 6) continue;
            countStr++;

            // Извлекаем год, месяц, день

            year = tokens[0].toInt();
            month = tokens[1].toInt();
            day = tokens[2].toInt();

            eop.epoch = QDateTime (QDate(year,month,day), QTime(0, 0, 0));

            eop.mjd = tokens[3].toDouble();
            eop.xp = tokens[4].toDouble()*_arcsec2Rad;
            eop.yp = tokens[5].toDouble()*_arcsec2Rad;
            eop.dUt1 = tokens[6].toDouble();
            eop.epochTai = calcTime(eop.epoch);
            if (countStr == 1){
                eopFirst = eop; //сохранить первый
            }

            if (eop.epoch >= begTimeUtc0 && eop.epoch <= endTimeUtc0) {
                listEop.append(eop);
            }

        }
        file.close();

        eopLast = eop; //запомнить последний считанный


        // данные считанные программой
        if (listEop.size()==0){
            //ни одно из имеющихся значений не попало в массив
            // значит интервал либо слишком рано, либо слишком поздно
            // следует заполнить listEop одни значением либо самым первым либо самым последним
            if (begTimeUtc0>eopLast.epoch){
                listEop.append(eopLast);
            }
            else if(endTimeUtc0<eopFirst.epoch){
                listEop.append(eopFirst);
            }
        }
        else {
            // найти общее число элементов которое должно быть по заданным условиям

            int countEop = begTimeUtc0.daysTo(endTimeUtc0) + 1;
            // если размер listEop меньше чем должно быть, то надо выставить признак
            if(listEop.size()<countEop){
                //в случае если используется промежуток близкий к концу интервала
                // то производится интерполяция на меньшей, неотцентрированной сетке
                outRange = true;
            }
        }
        //запомнить даты начала и конца массива
        begEpochUtc = listEop[0].epoch;
        endEpochUtc = listEop[listEop.size()-1].epoch;

}

void EarthOrientationParams::getEop(QDateTime curTimeUtc, eop_t &eop)
{

    // возвращает параметры ПВЗ в конкретный момент времени
    // метод интерполирует на момент curTimeUtc по данным listEop

    if (listEop.size() == 0 ) {
        //  ранее не считывалось необходимо считать
        getListEop(curTimeUtc,curTimeUtc);
        eop = interpolateEOP(curTimeUtc);
    }
    else {
        eop = interpolateEOP(curTimeUtc);
    }
}

eop_t EarthOrientationParams::interpolateEOP(QDateTime epochUTC)
{
    //получение времени в секундах таи от-но 2000
    long double epochTai = calcTime(epochUTC);


    // найти индекс приходящийся на середину интервала
    int beginIndex = -1;
    for (int i = 0; i < listEop.size()-1; i++) {
        if ((epochTai >= listEop[i].epochTai)&&(epochTai < listEop[i+1].epochTai)) {
            beginIndex = i;
        }
    }

    //подмассивы срезов для интерполяции
    QVector<long double> time, xp, yp, dUt1;

    if (beginIndex != -1 ) {
        // найдены данные в массиве listEop
        int interpDays = static_cast<int>(_interpOrder / 2);
        int index = beginIndex - interpDays;
        while (index <= beginIndex+interpDays) {

            index++;

            if ((index>=0)&&(index<listEop.size())){

                time.append(listEop[index].epochTai );
                dUt1.append(listEop[index].dUt1 );
                xp.append(listEop[index].xp );
                yp.append(listEop[index].yp );
            }
        }
    }
    else {
        // не найдены данные в массиве listEop
        // значит запрашиваемое значение выходит за пределы имеющихся данных
        // необходимо вернуть одно единственное значение - последнее
        // полюс замирает
        return listEop[listEop.size()-1];

    }


    //прогнозируемое значение
    eop_t eop;
    eop.epoch = epochUTC;
    eop.epochTai = epochTai;
    eop.xp = interpolation(time, xp, epochTai);
    eop.yp = interpolation(time, yp, epochTai);
    eop.dUt1 = interpolation(time, dUt1, epochTai);

    return eop;

}

long double EarthOrientationParams::calcTime(QDateTime epochUTC)
{
    //поправка между TAI и UTC
    int TAIUTC = TimeSystem::leapSeconds(epochUTC.date().year(), epochUTC.date().month());

    //время в TAI
    QDateTime epochTAI = epochUTC.addSecs(static_cast<long double>(TAIUTC));
    long double timeSecTAI = static_cast<long double>(_date010120.secsTo(epochTAI));
    // Время в секундах TAI от 01.01.2000 00:00:00
    return timeSecTAI;
}

long double EarthOrientationParams::interpolation(const QVector<long double> &X, const QVector<long double> &Y, const long double x)
{
    Q_ASSERT( X.size() == Y.size() );
    long double y(0);
    for ( int i = 0; i < X.size(); ++i ) {
        if (x == X[i] ) return Y[i];

        long double t(1.0);
        for ( int j = 0; j < X.size(); ++j ) {
            if ( i != j ) t *= (x-X[j])/(X[i]-X[j]);
        }
        y += t*Y[i];
    }
    return y;
}

QString EarthOrientationParams::getFileEop() const
{
    return fileName;
}

void EarthOrientationParams::setFileEop(const QString &value)
{
    fileName = value;
}
