QT       += network
QT       += gui

TARGET = QtwitterLib
TEMPLATE = lib

DEFINES += QTWITTERLIB_LIBRARY

HEADERS += QtwitterLib_global.h \
    oauth.h \
    signalwaiter.h \
    oauthtwitter.h \
    qtwitteruser.h \
    qtwitterstatus.h \
    qtwitternetbase.h \
    qtwitterhometimeline.h \
    qtwitterfriendstimeline.h \
    qtwitterusertimeline.h \
    qtwittermentions.h \
    qtwitterretweetbyme.h \
    qtwitterretweettome.h \
    qtwitterretweetsofme.h \
    qtwitterstatusshow.h \
    qtwstatusupdate.h

SOURCES += \
    oauth.cpp \
    signalwaiter.cpp \
    oauthtwitter.cpp \
    qtwitteruser.cpp \
    qtwitterstatus.cpp \
    qtwitternetbase.cpp \
    qtwitterhometimeline.cpp \
    qtwitterfriendstimeline.cpp \
    qtwitterusertimeline.cpp \
    qtwittermentions.cpp \
    qtwitterretweetbyme.cpp \
    qtwitterretweettome.cpp \
    qtwitterretweetsofme.cpp \
    qtwitterstatusshow.cpp \
    qtwstatusupdate.cpp
