QT       += core gui network

TARGET = pinauthstatusupdate
TEMPLATE = app
INCLUDEPATH += ../../src

HEADERS += \
    statusupdatewidget.h

SOURCES += \
    statusupdatewidget.cpp \
    main.cpp

FORMS += \
    statusupdatewidget.ui

symbian: LIBS += -lqtweetlib
else:unix|win32: LIBS += -L$$OUT_PWD/../../lib/ -lqtweetlib

INCLUDEPATH += $$PWD/../../src
DEPENDPATH += $$PWD/../../lib
