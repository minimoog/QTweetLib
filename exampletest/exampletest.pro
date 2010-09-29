QT       += core gui network

TARGET = exampletest
TEMPLATE = app
win32:LIBS += ../lib/QTweetLib.lib
unix:LIBS += -L../lib -lQTweetLib

unix:QMAKE_LFLAGS += -Wl,-rpath=../lib

CONFIG += link_prl

INCLUDEPATH += ../src

SOURCES += main.cpp\
        mainwindow.cpp \

HEADERS  += mainwindow.h \
    
FORMS    += mainwindow.ui
