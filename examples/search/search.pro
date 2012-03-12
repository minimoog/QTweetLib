QT       += core gui network

TARGET = searchexample
TEMPLATE = app
win32:LIBS += ../../lib/QTweetLib.lib
INCLUDEPATH += ../../src

SOURCES +=\
    mainwindow.cpp \
    main.cpp

HEADERS  += \
    mainwindow.h
    

FORMS += \
    mainwindow.ui

symbian: LIBS += -lqtweetlib
else:unix|win32: LIBS += -L$$OUT_PWD/../../lib/ -lqtweetlib

INCLUDEPATH += $$PWD/../../src
DEPENDPATH += $$PWD/../../lib
