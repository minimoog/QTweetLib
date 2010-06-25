QT       += network
QT       += gui

TARGET = QTwitLib
TEMPLATE = lib

DEFINES += QTWITLIB_LIBRARY

HEADERS += \
    oauth.h \
    signalwaiter.h \
    oauthtwitter.h \
    qtwitlib_global.h \
    qtwitfriendstimeline.h \
    qtwithometimeline.h \
    qtwitmentions.h \
    qtwitnetbase.h \
    qtwitretweetbyme.h \
    qtwitretweetsofme.h \
    qtwitretweettome.h \
    qtwitstatus.h \
    qtwitstatusshow.h \
    qtwituser.h \
    qtwitusertimeline.h \
    qtwitstatusupdate.h

SOURCES += \
    oauth.cpp \
    signalwaiter.cpp \
    oauthtwitter.cpp \
    qtwitfriendstimeline.cpp \
    qtwithometimeline.cpp \
    qtwitmentions.cpp \
    qtwitnetbase.cpp \
    qtwitretweetbyme.cpp \
    qtwitretweetsofme.cpp \
    qtwitretweettome.cpp \
    qtwitstatus.cpp \
    qtwitstatusshow.cpp \
    qtwituser.cpp \
    qtwitusertimeline.cpp \
    qtwitstatusupdate.cpp
