QT       += core gui network

TARGET = georeverse
TEMPLATE = app
win32:LIBS += ../../lib/QTweetLib.lib
unix:LIBS += -L$$OUT_PWD/../../lib/ -lqtweetlib
INCLUDEPATH += $$PWD/../../src
DEPENDPATH += $$PWD/../../src
CONFIG += mobility
MOBILITY += location

SOURCES += \
    main.cpp \
    georeverse.cpp

HEADERS += \
    georeverse.h

FORMS += \
    georeverse.ui
