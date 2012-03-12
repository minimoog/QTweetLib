QT       += core gui network

TARGET = pinauthstatusupdate
TEMPLATE = app
INCLUDEPATH += ../../src

HEADERS += \
    statusupdatewidget.h \
    pindialog.h \
    myoauthtwitter.h

SOURCES += \
    statusupdatewidget.cpp \
    pindialog.cpp \
    myoauthtwitter.cpp \
    main.cpp

FORMS += \
    statusupdatewidget.ui \
    pindialog.ui

symbian: LIBS += -lqtweetlib
else:unix|win32: LIBS += -L$$OUT_PWD/../../lib/ -lqtweetlib

INCLUDEPATH += $$PWD/../../src
DEPENDPATH += $$PWD/../../lib
