#include "propagation.h"
#include <iostream>
#include<iomanip>

Propagation::Propagation() {

}

void Propagation::init( input_data_propagation_t inputData ) {
    // !!!ЛЕВАЯ ГРАНИЦА ПРОГНОЗА ДОЛЖНА БЫТЬ МЕНЬШЕ ПРАВОЙ!!!

    //принимаем в эпохе UTC
    _beginEpoch = inputData.t0;
    _beginForecast = inputData.ts;
    _endForecast = inputData.tf;
    step = inputData.step;

    //настройки интегратора
    _integrator.setNs( ( inputData.integrOrder + 2 ) / 2.0 );
    _integrator.setNi( 3 );
    _integrator.setStep( inputData.integrStep );
    _integrator.setEtolErr( inputData.integrEtol );
    if ( inputData.integrEtol == 0.0 ) _integrator.setEtolErr( 0.0 ); // Для интегрирования с постоянным шагом

    //  время в TAI
    long double TAIUTC; //  поправка между TAI и UTC
    TAIUTC = TimeSystem::leapSeconds( _beginEpoch.date().year(), _beginEpoch.date().month() );
    _beginEpochTai = _beginEpoch.addSecs( TAIUTC );

    TAIUTC = TimeSystem::leapSeconds( _beginForecast.date().year(), _beginForecast.date().month() );
    _beginForecastTai = _beginForecast.addSecs( TAIUTC );

    TAIUTC = TimeSystem::leapSeconds( _endForecast.date().year(), _endForecast.date().month() );
    _endForecastTai = _endForecast.addSecs( TAIUTC );

    //поиск минимальной и максимальной эпохи
    //TAI
    _minEpochTai = _beginForecastTai;
    if ( _beginEpochTai < _minEpochTai ) _minEpochTai = _beginEpochTai;
    _maxEpochTai = _endForecastTai;
    if ( _maxEpochTai < _beginEpochTai ) _maxEpochTai = _beginEpochTai;

    //UTC
    _minEpochUtc = _beginForecast;
    if ( _beginEpoch < _minEpochUtc ) _minEpochUtc = _beginEpoch;
    _maxEpochUtc = _endForecast;
    if ( _maxEpochUtc < _beginEpoch ) _maxEpochUtc = _beginEpoch;


    //  рассчитать сетку с заданным шагом до нужной эпохи
    fillGridTime( _beginEpochTai, _beginForecastTai, _endForecastTai, step, _gridTime0 );


    _integrator.perturbations.setPertTides( inputData.pertTides );
    // настройка структуры возмущений

    if ( inputData.GeoNM >= 2 && inputData.GeoMN >= 0 ) {
        _integrator.perturbations.setPertNM( inputData.GeoNM + 3 );
        _integrator.perturbations.setPertMN( inputData.GeoMN + 2 );
        _integrator.perturbations.setPertEarthHarm( true );

        if ( fileEop != "" ) {
            // Если название файла с ПВЗ не пустое
            _integrator.perturbations.setFileEop( fileEop );;
            //инициализация массивов геопотенциала
            _integrator.perturbations.initGeoConst();
            //инициализация данных пвз
            _integrator.perturbations.initEop( _minEpochUtc, _maxEpochUtc );
        } else {
            //файл с данными ПВЗ не задан
            qDebug() << "Error: the data file for the earth's rotation parameters has not been set";
            qDebug() << "The influence of geopotential harmonics will be disabled!";
            _integrator.perturbations.setPertEarthHarm( false );
        }
    } else {
        _integrator.perturbations.setPertEarthHarm( false );
    }

    //световое давление
    if ( inputData.pertRadiation ) {
        _integrator.perturbations.setPertRadPressure( true );
        _integrator.perturbations.setLightCoef( inputData.lightCoef );
    }

    if ( fileFond != "" ) {
        //файл фонда задан
        _integrator.perturbations.setFileFond( fileFond );
        _integrator.perturbations.setPertSun( inputData.pertSun );
        _integrator.perturbations.setPertMoon( inputData.pertMoon );
        _integrator.perturbations.setPertPlanet( inputData.pertPlanets );
        _integrator.perturbations.setPertRelativistic( inputData.pertRelatEffects );
    } else {
        //файл фонда не задан
        qDebug() << "Error: the data file fond has not been set";
        if ( inputData.pertSun ) {
            qDebug() << "The influence of gravity of Sun will be disabled!";
        }
        if ( inputData.pertMoon ) {
            qDebug() << "The influence of gravity of Moon will be disabled!";
        }
        if ( inputData.pertPlanets ) {
            qDebug() << "The influence of gravity of planets will be disabled!";
        }
        if ( inputData.pertRelatEffects ) {
            qDebug() << "The influence of Relativistic Effects will be disabled!";
        }
    }

    //определение количества параметров
    setX0( inputData.x );
    setY0( inputData.y );

    //Настройки количества интегрируемых уравнений
    if ( inputData.x.size() == inputData.y.size() ) {
        _integrator.setNxy( inputData.x.size() );
    } else {
        _integrator.setNxy( min( inputData.x.size(), inputData.y.size() ) );
    }

    //прогнозирование без уточнения
    _integrator.perturbations.setBeginEpochTai( _beginEpochTai );
    _integrator.perturbations.setBeginEpochUtc( _beginEpoch );

}


void Propagation::calculate() {
    // Размер всего поступившего массива измерений
    int nMes0 = _gridTime0.size();
    _ephemeris.resize( _gridTime0.size() ); // задаем размер как у входящей сетки времени

    // Временный массив для извлечения из интегратора
    QVector<ephemeris_t> tempEph;

    // --- Обратное интегрирование ---
    // Найти (если есть) интервал с отрицательным временем
    _gridTime.clear();
    for ( int i = 0; i < nMes0; i++ ) {
        if ( _gridTime0[i] <= 0.0 ) {
            _gridTime.append( _gridTime0[i] );
        }
    }

    // Если нашелся отрицательный временной промежуток, то интегрировать назад
    if ( (_gridTime.size() > 1) || ( _gridTime.size() == 1 && _gridTime[0] != 0.0) ) {
        _integrator.setX( x0 );
        _integrator.setY( y0 );
        _integrator.lobbie( _gridTime ); // интегрирование назад (_gridTime <= 0)

        tempEph = _integrator.getEphemeris();  // получение эфемерид


        // Заполнение части результирующего массива с эфемеридами (от начала)
        for ( int i = 0; i < tempEph.size(); i++ ) {
            // _ephemeris[i] = tempEph[tempEph.size() - 1 - i];
            _ephemeris[i] = tempEph[i]; // Нужен обратный порядок во времени для уточнения --------------------- Правка модели
        }

        _integrator.integratorClear();
        tempEph.clear();

    }
    // --- Обратное интегрирование ---


    // --- Прямое интегрирование ---
    // Найти (если есть) интервал с положительным временем
    _gridTime.clear();
    for ( int i = 0; i < nMes0; i++ ) {
        if ( _gridTime0[i] >= 0.0 ) {
            _gridTime.append( _gridTime0[i] );
        }
    }

    // Если нашелся положительный временной промежуток, то интегрировать вперед
    if ((_gridTime.size() > 1) || ( _gridTime.size() == 1 && _gridTime[0] != 0.0)) {
        _integrator.setX( x0 );
        _integrator.setY( y0 );
        _integrator.lobbie( _gridTime ); // интегрирование вперед (_gridTime >= 0)

        tempEph = _integrator.getEphemeris();  // получение эфемерид

        // Заполнение части результирующего массива с эфемеридами (с конца)
        for ( int i = 0; i < tempEph.size(); i++ ) {
            _ephemeris[nMes0 - 1 - i] = tempEph[tempEph.size() - 1 - i];
        }
        _integrator.integratorClear();
        tempEph.clear();

    }
    // --- Прямое интегрирование ---

    //Заполнить календарную дату эфемерид UTC
    for ( int i = 0; i < nMes0; i++ ) {
        const long double tSec = _ephemeris[i].time;
        const qint64 msecs = static_cast<qint64>( qRound64( static_cast<qreal>( tSec * 1000.0L ) ) );
        _ephemeris[i].epoch = _beginEpoch.addMSecs( msecs );
    }
}



QVector<long double> Propagation::getX0() const {
    return x0;
}

void Propagation::setX0( const QVector<long double>& value ) {
    x0 = value;
}

QVector<long double> Propagation::getY0() const {
    return y0;
}

void Propagation::setY0( const QVector<long double>& value ) {
    y0 = value;
}

void Propagation::fillGridTime(QDateTime beginEpoch, QDateTime beginForecast, QDateTime endForecast, long double step, QVector<long double> &gridTime)
{
    // 1. Получаем "чистые" миллисекунды от начала эпохи Unix (UTC)
        // Это исключает любые проблемы с часовыми поясами и пересчетами JD
        long long msStart = beginEpoch.toMSecsSinceEpoch();
        long long msForecastBegin = beginForecast.toMSecsSinceEpoch();
        long long msForecastEnd = endForecast.toMSecsSinceEpoch();

        // 2. Считаем интервалы в секундах (целое число мс / 1000.0)
        long double t_start = static_cast<long double>(msForecastBegin - msStart) / 1000.0L;
        long double t_end = static_cast<long double>(msForecastEnd - msStart) / 1000.0L;

        long double duration = t_end - t_start;

        // 3. Количество точек
        // Используем очень маленький эпсилон, чтобы избежать ошибок округления float
        int nobs = static_cast<int>(duration / step + 1e-11L) + 1;

        gridTime.clear(); // Очищаем перед использованием
        gridTime.resize(nobs);

        // 4. Заполнение
        for (int i = 0; i < nobs; i++) {
            gridTime[i] = t_start + static_cast<long double>(i) * step;
            //cout << fixed << std::setprecision(20) << gridTime[i] << endl;
        }

        // 5. Жесткая фиксация конца
        if (nobs > 0) {
            gridTime[nobs - 1] = t_end;
        }
}


QVector<long double> Propagation::getX() const {
    return _integrator.getX();
}
QVector<long double> Propagation::getY() const {
    return _integrator.getY();
}

void Propagation::propagationClear() {
    _gridTime.clear();
    _gridTime0.clear();
    _gridTimeUTC.clear();

    _integrator.integratorClear();
}

void Propagation::setFileFond( const string& value ) {
    fileFond = value;
}

string Propagation::getFileFond() const {
    return fileFond;
}

void Propagation::setFileEop( const QString& value ) {
    fileEop = value;
}

QString Propagation::getFileEop() const {
    return _integrator.perturbations.getFileEop();
}

QVector<ephemeris_t> Propagation::getEphemeris() const {
    return _ephemeris;
}

void Propagation::initImprove() {
    //Инициализация начальных условий для запуска интегрирования с частными производными


}

void Propagation::calculateImprove() {
    //Запуск интегрирования с частными производными
    // Размер всего поступившего массива измерений
    int nMes0 = _gridTime0.size();
    xGrid.resize( nMes0 ); //Размерность по первому индексу соответствует моментам времени
    yGrid.resize( nMes0 ); //Размерность по второму индексу соответствует кол-ву интегрируемых параметров

    // Временный массив для извлечения из интегратора
    QVector<QVector<long double>> tempX, tempY;

    _gridTime.clear();
    for ( int i = 0; i < nMes0; i++ ) {
        if ( _gridTime0[i] <= 0.0 ) {
            _gridTime.append( _gridTime0[i] );
        }
    }

    // Если нашелся отрицательный временной промежуток, то интегрировать назад
    if ( _gridTime.size() > 1 ) {
        _integrator.setX( x0 );
        _integrator.setY( y0 );
        _integrator.lobbie( _gridTime ); // интегрирование назад (_gridTime <= 0)

        tempX = _integrator.getXGrid();  // получение интегрируемых значений
        tempY = _integrator.getYGrid();


        // Заполнение части результирующего массива с эфемеридами (от начала)
        for ( int i = 0; i < tempX.size(); i++ ) {
            xGrid[i] = tempX[tempX.size() - 1 - i];
            yGrid[i] = tempY[tempY.size() - 1 - i];
        }
        //Очищение
        _integrator.integratorClear();
        tempX.clear();
        tempY.clear();

    }
    // --- Обратное интегрирование ---

    // --- Прямое интегрирование ---
    // Найти (если есть) интервал с положительным временем
    _gridTime.clear();
    for ( int i = 0; i < nMes0; i++ ) {
        if ( _gridTime0[i] >= 0.0 ) {
            _gridTime.append( _gridTime0[i] );
        }
    }

    // Если нашелся положительный временной промежуток, то интегрировать вперед
    if ( _gridTime.size() > 1 ) {
        _integrator.setX( x0 );
        _integrator.setY( y0 );
        _integrator.lobbie( _gridTime ); // интегрирование вперед (_gridTime >= 0)

        tempX = _integrator.getXGrid();  // получение интегрируемых значений
        tempY = _integrator.getYGrid();  // получение первых производных интегрируемых значений

        // Заполнение части результирующего массива с эфемеридами (с конца)
        for ( int i = 0; i < tempX.size(); i++ ) {
            xGrid[nMes0 - 1 - i] = tempX[tempX.size() - 1 - i];
            yGrid[nMes0 - 1 - i] = tempY[tempY.size() - 1 - i];
        }
        _integrator.integratorClear();
        tempX.clear();
        tempY.clear();
    }
    // --- Прямое интегрирование ---

}

QVector<QVector<long double> > Propagation::getImprove() const {
    //Возвращаем матрицу частных производных

    return xGrid;

}
