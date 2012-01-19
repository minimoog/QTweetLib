QT       += core gui network

TARGET = pinauthstatusupdate
TEMPLATE = app
win32:LIBS += ../../lib/QTweetLib.lib
unix:LIBS += -L$$OUT_PWD/../../lib/ -lqtweetlib
INCLUDEPATH += $$PWD/../../src
DEPENDPATH += $$PWD/../../src

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
