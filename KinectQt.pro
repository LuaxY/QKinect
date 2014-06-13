#-------------------------------------------------
#
# Project created by QtCreator 2014-04-19T11:38:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KinectQt
TEMPLATE = app


SOURCES += main.cpp\
        programme.cpp \
    customevent.cpp \
    drawzone.cpp \
    qkinect.cpp \
    TrameDMX.cpp \
    about.cpp

HEADERS  += programme.h \
    customevent.h \
    drawzone.h \
    qkinect.h \
    TrameDMX.h \
    about.h

FORMS    += programme.ui \
    about.ui

LIBS     += C:/Kinect/lib/x86/Kinect10.lib
LIBS     += C:/scratch/DMX/i386/ftd2xx.lib

INCLUDEPATH += C:/Kinect/inc
INCLUDEPATH += C:/scratch/DMX

RESOURCES += \
    ressources/ressources.qrc
