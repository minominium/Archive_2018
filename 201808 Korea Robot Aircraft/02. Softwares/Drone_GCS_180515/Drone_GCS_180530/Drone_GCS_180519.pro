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
