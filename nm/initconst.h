#ifndef INITCONST_H
#define INITCONST_H

#include <cmath>
#include <QVector>

class InitConst {




public:
    // Общая инициализация, которая вызывается в самом начале
    static void generalInit();

    // Гравитацонные параметры планет, Солнца и Луны (км^3/c^2)
    static constexpr const long double gravParam[11] = {
        132712440041.939377,        //  0, Солнце
               22032.090000,        //  1, Меркурий
              324858.592000,        //  2, Венера
              398600.441800,        //  3, Земля
               42828.375214,        //  4, Марс
           126712764.800000,        //  5, Юпитер
            37940585.200000,        //  6, Сатурн
             5794548.600000,        //  7, Уран
             6836535.000000,        //  8, Нептун
                 977.000000,        //  9, Плутон
                4902.800110         // 10, Луна
    };

    // Экваториальные радиусы (км)
    static constexpr const long double raduisEarth = 6378.1366;  // Земля
    static constexpr const long double raduisMoon = 1738.0;      // Луна

    // Гармоники геопотенциала
    static long double geoHarmsOriginal[][4];   // читабельный исходный список гармоник, из него заполняются массивы для модели
    static long double geoHarms[3][25][25];     // неизменямый массив, используется в модели
    static long double geoHarmsPert[3][25][25]; // используется в алгоритме Каннингема и приливах, нестандартное расположение гармоник

    //массивы для геопотенциала
    static constexpr long double gNorm = 9.79828670817142398772E-03;                          // заполняется в geoConst

    static long double CV[2][25][25];
    static long double Q[23][23];
    static long double QX[23][23];
    static long double QZ[23][23];
    static long double CNST[23][24];
    static long double SNST[23][24];

    void geoConst(int NM, int MN) ;
    static long double q[25][25], qx[25][25], qz[25][25];




};

enum CelectialBodyNum {
    SUN     = 0,
    MERCURY = 1,
    VENUS   = 2,
    EARTH   = 3,
    MARS    = 4,
    JUPITER = 5,
    SATURN  = 6,
    URANUS  = 7,
    NEPTUNE = 8,
    PLUTO   = 9,
    MOON    = 10
};

#endif // INITCONST_H
