QT       += core gui network

TARGET = statusupdate
TEMPLATE = app
win32:LIBS += ../../lib/QTweetLib.lib
INCLUDEPATH += ../../src

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

symbian: LIBS += -lqtweetlib
else:unix|win32: LIBS += -L$$OUT_PWD/../../lib/ -lqtweetlib

INCLUDEPATH += $$PWD/../../src
DEPENDPATH += $$PWD/../../lib
