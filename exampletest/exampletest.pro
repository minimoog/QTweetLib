QT       += core gui network

TARGET = exampletest
TEMPLATE = app
LIBS += ../lib/QTweetLib.lib
CONFIG += link_prl

INCLUDEPATH += ../src

SOURCES += main.cpp\
        mainwindow.cpp \

HEADERS  += mainwindow.h \
    
FORMS    += mainwindow.ui
