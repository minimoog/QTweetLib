QT       += core gui network

TARGET = geosearch
TEMPLATE = app
win32:LIBS += ../../lib/QTweetLib.lib
unix:LIBS += -L$$OUT_PWD/../../lib/ -lqtweetlib
INCLUDEPATH += $$PWD/../../src
DEPENDPATH += $$PWD/../../src
CONFIG += mobility
MOBILITY += location

SOURCES += \
    main.cpp \
    geosearch.cpp

HEADERS += \
    geosearch.h

FORMS += \
    geosearch.ui
