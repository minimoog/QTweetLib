QT       += network
QT       -= gui

TARGET = QtwitterLib
TEMPLATE = lib

DEFINES += QTWITTERLIB_LIBRARY

HEADERS += QtwitterLib_global.h \
    oauth.h

SOURCES += \
    oauth.cpp
