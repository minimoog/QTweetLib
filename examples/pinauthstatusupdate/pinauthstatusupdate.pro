QT       += core gui network

TARGET = pinauthstatusupdate
TEMPLATE = app
LIBS += ../../lib/QTweetLib.lib
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
