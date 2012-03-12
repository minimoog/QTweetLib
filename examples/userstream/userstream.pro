QT       += core gui network

TARGET = userstream
TEMPLATE = app
win32:LIBS += ../../lib/QTweetLib.lib
INCLUDEPATH += ../../src


SOURCES += \
    main.cpp \
    userstream.cpp

HEADERS += \
    userstream.h

FORMS += \
    userstream.ui

symbian: LIBS += -lqtweetlib
else:unix|win32: LIBS += -L$$OUT_PWD/../../lib/ -lqtweetlib

INCLUDEPATH += $$PWD/../../src
DEPENDPATH += $$PWD/../../lib
