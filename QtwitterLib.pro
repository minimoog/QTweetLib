QT       += network
QT       += gui

TARGET = QtwitterLib
TEMPLATE = lib

DEFINES += QTWITTERLIB_LIBRARY

HEADERS += QtwitterLib_global.h \
    oauth.h \
    signalwaiter.h \
    oauthtwitter.h \
    qtwitteruser.h

SOURCES += \
    oauth.cpp \
    signalwaiter.cpp \
    oauthtwitter.cpp \
    qtwitteruser.cpp
