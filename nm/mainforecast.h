#ifndef MAINFORECAST_H
#define MAINFORECAST_H

#include "inputdata.h"

class MainForecast {
public:
    MainForecast();

    static void forecast(input_data_t* inputData);
};

#endif // MAINFORECAST_H
