#-------------------------------------------------
#
# Project created by QtCreator 2016-07-18T22:40:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled
TEMPLATE = app


SOURCES += main.cpp\
    mypoint.cpp \
    myview.cpp \
    myscene.cpp \
    mytable.cpp \
    mainwindow.cpp \
    myaxes.cpp

HEADERS  += mainwindow.h \
    mypoint.h \
    myview.h \
    myscene.h \
    mytable.h \
    myaxes.h

FORMS    += mainwindow.ui

RESOURCES += \
    myres.qrc
