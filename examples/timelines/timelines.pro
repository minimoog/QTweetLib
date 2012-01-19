QT       += core gui network

TARGET = timelines
TEMPLATE = app
win32:LIBS += ../../lib/QTweetLib.lib
unix:LIBS += -L$$OUT_PWD/../../lib/ -lqtweetlib
INCLUDEPATH += $$PWD/../../src
DEPENDPATH += $$PWD/../../src

SOURCES += \
    mainwindow.cpp \
    main.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui
