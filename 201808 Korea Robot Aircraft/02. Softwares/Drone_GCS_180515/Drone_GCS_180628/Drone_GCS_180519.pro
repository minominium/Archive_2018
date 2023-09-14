#-------------------------------------------------
#
# Project created by QtCreator 2018-05-19T02:31:31
#
#-------------------------------------------------

QT       += core gui webkitwidgets serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Drone_GCS_180519
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    spline.h

FORMS    += mainwindow.ui


#INCLUDEPATH += ../Drone_GCS_180519/alglib

#SOURCES += \
#    ../Drone_GCS_180519/alglib/statistics.cpp \
#    ../Drone_GCS_180519/alglib/specialfunctions.cpp \
#    ../Drone_GCS_180519/alglib/solvers.cpp \
#    ../Drone_GCS_180519/alglib/optimization.cpp \
#    ../Drone_GCS_180519/alglib/linalg.cpp \
#    ../Drone_GCS_180519/alglib/interpolation.cpp \
#    ../Drone_GCS_180519/alglib/integration.cpp \
#    ../Drone_GCS_180519/alglib/fasttransforms.cpp \
#    ../Drone_GCS_180519/alglib/diffequations.cpp \
#    ../Drone_GCS_180519/alglib/dataanalysis.cpp \
#    ../Drone_GCS_180519/alglib/ap.cpp \
#    ../Drone_GCS_180519/alglib/alglibmisc.cpp \
#    ../Drone_GCS_180519/alglib/alglibinternal.cpp

#HEADERS += \
#    ../Drone_GCS_180519/alglib/statistics.h \
#    ../Drone_GCS_180519/alglib/specialfunctions.h \
#    ../Drone_GCS_180519/alglib/solvers.h \
#    ../Drone_GCS_180519/alglib/optimization.h \
#    ../Drone_GCS_180519/alglib/linalg.h \
#    ../Drone_GCS_180519/alglib/interpolation.h \
#    ../Drone_GCS_180519/alglib/integration.h \
#    ../Drone_GCS_180519/alglib/fasttransforms.h \
#    ../Drone_GCS_180519/alglib/diffequations.h \
#    ../Drone_GCS_180519/alglib/dataanalysis.h \
#    ../Drone_GCS_180519/alglib/ap.h \
#    ../Drone_GCS_180519/alglib/alglibmisc.h \
#    ../Drone_GCS_180519/alglib/alglibinternal.h
