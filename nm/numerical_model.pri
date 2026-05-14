
include(sofa/sofa.pri)

HEADERS += \
    nm/DataTypes.h \
    nm/earthorientationparams.h \
    nm/timesystem.h \
    nm/initconst.h \
    nm/integrator.h \
    nm/perturbations.h \
    nm/propagation.h \
    nm/matutils.h \
#    nm\numericalmodel.h \

SOURCES += \
    nm/earthorientationparams.cpp \
    nm/timesystem.cpp \
    nm/initconst.cpp \
    nm/integrator.cpp \
    nm/perturbations.cpp \
    nm/propagation.cpp \
    nm/matutils.cpp \
#    nm\numericalmodel.cpp \
