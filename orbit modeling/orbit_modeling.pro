QT       += core gui
QT += concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += qwt

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += core orbit \

SOURCES += \
    cluster.cpp \
    core/cEci.cpp \
    core/cJulian.cpp \
    core/cSite.cpp \
    core/cTLE.cpp \
    core/cVector.cpp \
    core/coord.cpp \
    core/globals.cpp \
    core/stdafx.cpp \
    main.cpp \
    mainwindow.cpp \
    orbit/cNoradBase.cpp \
    orbit/cNoradSDP4.cpp \
    orbit/cNoradSGP4.cpp \
    orbit/cOrbit.cpp \
    orbit/cSatellite.cpp \
    orbit/stdafx.cpp \
    point.cpp \
    world.cpp

HEADERS += \
    cluster.h \
    core/cEci.h \
    core/cJulian.h \
    core/cSite.h \
    core/cTLE.h \
    core/cVector.h \
    core/coord.h \
    core/coreLib.h \
    core/exceptions.h \
    core/globals.h \
    core/stdafx.h \
    mainwindow.h \
    orbit/cNoradBase.h \
    orbit/cNoradSDP4.h \
    orbit/cNoradSGP4.h \
    orbit/cOrbit.h \
    orbit/cSatellite.h \
    orbit/orbitLib.h \
    orbit/stdafx.h \
    point.h \
    world.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    StarLink.txt \
    Falcon.txt \
    ISSDEB.txt

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../Qwt-6.2.0/lib/ -lqwt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../Qwt-6.2.0/lib/ -lqwtd
else:unix: LIBS += -L$$PWD/../../../../Qwt-6.2.0/lib/ -lqwt

INCLUDEPATH += $$PWD/../../../../Qwt-6.2.0/include
DEPENDPATH += $$PWD/../../../../Qwt-6.2.0/include
