QT       += core gui network

TARGET = userstream
TEMPLATE = app
win32:LIBS += ../../lib/QTweetLib.lib
unix:LIBS += -L$$OUT_PWD/../../lib/ -lqtweetlib
INCLUDEPATH += $$PWD/../../src
DEPENDPATH += $$PWD/../../src

SOURCES += \
    main.cpp \
    userstream.cpp

HEADERS += \
    userstream.h

FORMS += \
    userstream.ui
