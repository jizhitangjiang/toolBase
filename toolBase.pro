#-------------------------------------------------
#
# Project created by QtCreator 2019-06-19T17:10:36
#
#-------------------------------------------------

QT       += network widgets

QT       -= gui

TARGET = toolBase
TEMPLATE = lib
CONFIG += staticlib

DESTDIR = $$PWD/lib

INCLUDEPATH += $$PWD/include

unix {
    target.path = /usr/lib
    INSTALLS += target
}

SOURCES += \
    src/widget/WindowStyle.cpp \
    src/config/Setting.cpp \
    src/network/NetWork.cpp

HEADERS += \
    include/WindowStyle.h \
    include/Setting.h \
    include/NetWork.h
