#-------------------------------------------------
#
# Project created by QtCreator 2019-06-20T16:27:39
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = example
TEMPLATE = app

DESTDIR = $$PWD/../bin

INCLUDEPATH = $$PWD/../../include

LIBS += -L$$PWD/../../lib -ltoolBase

SOURCES += main.cpp\
        Widget.cpp

HEADERS  += Widget.h

FORMS    += Widget.ui
