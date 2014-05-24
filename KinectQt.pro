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
    qkinect.cpp

HEADERS  += programme.h \
    customevent.h \
    drawzone.h \
    qkinect.h

FORMS    += programme.ui

LIBS     += C:/Kinect/lib/x86/Kinect10.lib

INCLUDEPATH += C:/Kinect/inc
#DEPENDPATH  += C:/Kinect/inc/

RESOURCES += \
    ressources/ressources.qrc
