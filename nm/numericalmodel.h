#ifndef NUMERICALMODEL_H
#define NUMERICALMODEL_H

#include "determination.h"
#include "propagation.h"

class NumericalModel {

private:
    Determination determination;

public:
    NumericalModel();

    void start(int);
};

#endif // NUMERICALMODEL_H
