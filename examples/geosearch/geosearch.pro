QT       += core gui network

TARGET = geosearch
TEMPLATE = app
win32:LIBS += ../../lib/QTweetLib.lib
INCLUDEPATH += ../../src
CONFIG += mobility
MOBILITY += location

SOURCES += \
    main.cpp \
    geosearch.cpp

HEADERS += \
    geosearch.h

FORMS += \
    geosearch.ui

symbian: LIBS += -lqtweetlib
else:unix|win32: LIBS += -L$$OUT_PWD/../../lib/ -lqtweetlib

INCLUDEPATH += $$PWD/../../src
DEPENDPATH += $$PWD/../../lib
