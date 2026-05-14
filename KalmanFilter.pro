QT += core gui
QT += sql
QT += widgets printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += "boost_1_90_0"

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QT += concurrent

SOURCES += \
    DBWorker/dbworker.cpp \
    UtochnenieSimple/Modules.cpp \
    UtochnenieSimple/utochnenie.cpp \
    UtochnenieSimple/utochneniesimple.cpp \
    app.cpp \
    chartdialog.cpp \
    choosesatelliteswindow.cpp \
    groupvisibilitywindow.cpp \
    main.cpp \
    mainwindow.cpp \
    nm/earthorientationparams.cpp \
    nm/initconst.cpp \
    nm/integrator.cpp \
    nm/matutils.cpp \
    nm/perturbations.cpp \
    nm/propagation.cpp \
    nm/sofa/a2af.c \
    nm/sofa/a2tf.c \
    nm/sofa/ab.c \
    nm/sofa/ae2hd.c \
    nm/sofa/af2a.c \
    nm/sofa/anp.c \
    nm/sofa/anpm.c \
    nm/sofa/apcg.c \
    nm/sofa/apcg13.c \
    nm/sofa/apci.c \
    nm/sofa/apci13.c \
    nm/sofa/apco.c \
    nm/sofa/apco13.c \
    nm/sofa/apcs.c \
    nm/sofa/apcs13.c \
    nm/sofa/aper.c \
    nm/sofa/aper13.c \
    nm/sofa/apio.c \
    nm/sofa/apio13.c \
    nm/sofa/atcc13.c \
    nm/sofa/atccq.c \
    nm/sofa/atci13.c \
    nm/sofa/atciq.c \
    nm/sofa/atciqn.c \
    nm/sofa/atciqz.c \
    nm/sofa/atco13.c \
    nm/sofa/atic13.c \
    nm/sofa/aticq.c \
    nm/sofa/aticqn.c \
    nm/sofa/atio13.c \
    nm/sofa/atioq.c \
    nm/sofa/atoc13.c \
    nm/sofa/atoi13.c \
    nm/sofa/atoiq.c \
    nm/sofa/bi00.c \
    nm/sofa/bp00.c \
    nm/sofa/bp06.c \
    nm/sofa/bpn2xy.c \
    nm/sofa/c2i00a.c \
    nm/sofa/c2i00b.c \
    nm/sofa/c2i06a.c \
    nm/sofa/c2ibpn.c \
    nm/sofa/c2ixy.c \
    nm/sofa/c2ixys.c \
    nm/sofa/c2s.c \
    nm/sofa/c2t00a.c \
    nm/sofa/c2t00b.c \
    nm/sofa/c2t06a.c \
    nm/sofa/c2tcio.c \
    nm/sofa/c2teqx.c \
    nm/sofa/c2tpe.c \
    nm/sofa/c2txy.c \
    nm/sofa/cal2jd.c \
    nm/sofa/cp.c \
    nm/sofa/cpv.c \
    nm/sofa/cr.c \
    nm/sofa/d2dtf.c \
    nm/sofa/d2tf.c \
    nm/sofa/dat.c \
    nm/sofa/dtdb.c \
    nm/sofa/dtf2d.c \
    nm/sofa/eceq06.c \
    nm/sofa/ecm06.c \
    nm/sofa/ee00.c \
    nm/sofa/ee00a.c \
    nm/sofa/ee00b.c \
    nm/sofa/ee06a.c \
    nm/sofa/eect00.c \
    nm/sofa/eform.c \
    nm/sofa/eo06a.c \
    nm/sofa/eors.c \
    nm/sofa/epb.c \
    nm/sofa/epb2jd.c \
    nm/sofa/epj.c \
    nm/sofa/epj2jd.c \
    nm/sofa/epv00.c \
    nm/sofa/eqec06.c \
    nm/sofa/eqeq94.c \
    nm/sofa/era00.c \
    nm/sofa/fad03.c \
    nm/sofa/fae03.c \
    nm/sofa/faf03.c \
    nm/sofa/faju03.c \
    nm/sofa/fal03.c \
    nm/sofa/falp03.c \
    nm/sofa/fama03.c \
    nm/sofa/fame03.c \
    nm/sofa/fane03.c \
    nm/sofa/faom03.c \
    nm/sofa/fapa03.c \
    nm/sofa/fasa03.c \
    nm/sofa/faur03.c \
    nm/sofa/fave03.c \
    nm/sofa/fk425.c \
    nm/sofa/fk45z.c \
    nm/sofa/fk524.c \
    nm/sofa/fk52h.c \
    nm/sofa/fk54z.c \
    nm/sofa/fk5hip.c \
    nm/sofa/fk5hz.c \
    nm/sofa/fw2m.c \
    nm/sofa/fw2xy.c \
    nm/sofa/g2icrs.c \
    nm/sofa/gc2gd.c \
    nm/sofa/gc2gde.c \
    nm/sofa/gd2gc.c \
    nm/sofa/gd2gce.c \
    nm/sofa/gmst00.c \
    nm/sofa/gmst06.c \
    nm/sofa/gmst82.c \
    nm/sofa/gst00a.c \
    nm/sofa/gst00b.c \
    nm/sofa/gst06.c \
    nm/sofa/gst06a.c \
    nm/sofa/gst94.c \
    nm/sofa/h2fk5.c \
    nm/sofa/hd2ae.c \
    nm/sofa/hd2pa.c \
    nm/sofa/hfk5z.c \
    nm/sofa/icrs2g.c \
    nm/sofa/ir.c \
    nm/sofa/jd2cal.c \
    nm/sofa/jdcalf.c \
    nm/sofa/ld.c \
    nm/sofa/ldn.c \
    nm/sofa/ldsun.c \
    nm/sofa/lteceq.c \
    nm/sofa/ltecm.c \
    nm/sofa/lteqec.c \
    nm/sofa/ltp.c \
    nm/sofa/ltpb.c \
    nm/sofa/ltpecl.c \
    nm/sofa/ltpequ.c \
    nm/sofa/moon98.c \
    nm/sofa/num00a.c \
    nm/sofa/num00b.c \
    nm/sofa/num06a.c \
    nm/sofa/numat.c \
    nm/sofa/nut00a.c \
    nm/sofa/nut00b.c \
    nm/sofa/nut06a.c \
    nm/sofa/nut80.c \
    nm/sofa/nutm80.c \
    nm/sofa/obl06.c \
    nm/sofa/obl80.c \
    nm/sofa/p06e.c \
    nm/sofa/p2pv.c \
    nm/sofa/p2s.c \
    nm/sofa/pap.c \
    nm/sofa/pas.c \
    nm/sofa/pb06.c \
    nm/sofa/pdp.c \
    nm/sofa/pfw06.c \
    nm/sofa/plan94.c \
    nm/sofa/pm.c \
    nm/sofa/pmat00.c \
    nm/sofa/pmat06.c \
    nm/sofa/pmat76.c \
    nm/sofa/pmp.c \
    nm/sofa/pmpx.c \
    nm/sofa/pmsafe.c \
    nm/sofa/pn.c \
    nm/sofa/pn00.c \
    nm/sofa/pn00a.c \
    nm/sofa/pn00b.c \
    nm/sofa/pn06.c \
    nm/sofa/pn06a.c \
    nm/sofa/pnm00a.c \
    nm/sofa/pnm00b.c \
    nm/sofa/pnm06a.c \
    nm/sofa/pnm80.c \
    nm/sofa/pom00.c \
    nm/sofa/ppp.c \
    nm/sofa/ppsp.c \
    nm/sofa/pr00.c \
    nm/sofa/prec76.c \
    nm/sofa/pv2p.c \
    nm/sofa/pv2s.c \
    nm/sofa/pvdpv.c \
    nm/sofa/pvm.c \
    nm/sofa/pvmpv.c \
    nm/sofa/pvppv.c \
    nm/sofa/pvstar.c \
    nm/sofa/pvtob.c \
    nm/sofa/pvu.c \
    nm/sofa/pvup.c \
    nm/sofa/pvxpv.c \
    nm/sofa/pxp.c \
    nm/sofa/refco.c \
    nm/sofa/rm2v.c \
    nm/sofa/rv2m.c \
    nm/sofa/rx.c \
    nm/sofa/rxp.c \
    nm/sofa/rxpv.c \
    nm/sofa/rxr.c \
    nm/sofa/ry.c \
    nm/sofa/rz.c \
    nm/sofa/s00.c \
    nm/sofa/s00a.c \
    nm/sofa/s00b.c \
    nm/sofa/s06.c \
    nm/sofa/s06a.c \
    nm/sofa/s2c.c \
    nm/sofa/s2p.c \
    nm/sofa/s2pv.c \
    nm/sofa/s2xpv.c \
    nm/sofa/sepp.c \
    nm/sofa/seps.c \
    nm/sofa/sp00.c \
    nm/sofa/starpm.c \
    nm/sofa/starpv.c \
    nm/sofa/sxp.c \
    nm/sofa/sxpv.c \
    nm/sofa/t_sofa_c.c \
    nm/sofa/taitt.c \
    nm/sofa/taiut1.c \
    nm/sofa/taiutc.c \
    nm/sofa/tcbtdb.c \
    nm/sofa/tcgtt.c \
    nm/sofa/tdbtcb.c \
    nm/sofa/tdbtt.c \
    nm/sofa/test.c \
    nm/sofa/tf2a.c \
    nm/sofa/tf2d.c \
    nm/sofa/tpors.c \
    nm/sofa/tporv.c \
    nm/sofa/tpsts.c \
    nm/sofa/tpstv.c \
    nm/sofa/tpxes.c \
    nm/sofa/tpxev.c \
    nm/sofa/tr.c \
    nm/sofa/trxp.c \
    nm/sofa/trxpv.c \
    nm/sofa/tttai.c \
    nm/sofa/tttcg.c \
    nm/sofa/tttdb.c \
    nm/sofa/ttut1.c \
    nm/sofa/ut1tai.c \
    nm/sofa/ut1tt.c \
    nm/sofa/ut1utc.c \
    nm/sofa/utctai.c \
    nm/sofa/utcut1.c \
    nm/sofa/xy06.c \
    nm/sofa/xys00a.c \
    nm/sofa/xys00b.c \
    nm/sofa/xys06a.c \
    nm/sofa/zp.c \
    nm/sofa/zpv.c \
    nm/sofa/zr.c \
    nm/timesystem.cpp \
    qcustomplot.cpp \
    welcomewindow.cpp

HEADERS += \
    DBWorker/dbworker.h \
    UtochnenieSimple/Modules.h \
    UtochnenieSimple/utochnenie.h \
    UtochnenieSimple/utochneniesimple.h \
    app.h \
    chartdialog.h \
    choosesatelliteswindow.h \
    groupvisibilitywindow.h \
    mainwindow.h \
    nm/DataTypes.h \
    nm/earthorientationparams.h \
    nm/initconst.h \
    nm/integrator.h \
    nm/matutils.h \
    nm/perturbations.h \
    nm/propagation.h \
    nm/sofa/sofa.h \
    nm/sofa/sofam.h \
    nm/timesystem.h \
    qcustomplot.h \
    welcomewindow.h

FORMS += \
    choosesatelliteswindow.ui \
    groupvisibilitywindow.ui \
    mainwindow.ui \
    welcomewindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
