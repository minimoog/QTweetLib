#-------------------------------------------------
#
# Project created by QtCreator 2010-08-03T11:16:58
#
#-------------------------------------------------

QT       += core gui network

TARGET = exampletest
TEMPLATE = app

INCLUDEPATH += ../

SOURCES += main.cpp\
        mainwindow.cpp \
    ../signalwaiter.cpp \
    ../qtweetusertimeline.cpp \
    ../qtweetusershow.cpp \
    ../qtweetuserlookup.cpp \
    ../qtweetuser.cpp \
    ../qtweetstatusupdate.cpp \
    ../qtweetstatusshow.cpp \
    ../qtweetstatusretweets.cpp \
    ../qtweetstatusretweet.cpp \
    ../qtweetstatusdestroy.cpp \
    ../qtweetstatus.cpp \
    ../qtweetretweettome.cpp \
    ../qtweetretweetsofme.cpp \
    ../qtweetretweetbyme.cpp \
    ../qtweetnetbase.cpp \
    ../qtweetmentions.cpp \
    ../qtweethometimeline.cpp \
    ../qtweetfriendstimeline.cpp \
    ../qtweetdirectmessages.cpp \
    ../oauthtwitter.cpp \
    ../oauth.cpp

HEADERS  += mainwindow.h \
    ../signalwaiter.h \
    ../qtweetusertimeline.h \
    ../qtweetusershow.h \
    ../qtweetuserlookup.h \
    ../qtweetuser.h \
    ../qtweetstatusupdate.h \
    ../qtweetstatusshow.h \
    ../qtweetstatusretweets.h \
    ../qtweetstatusretweet.h \
    ../qtweetstatusdestroy.h \
    ../qtweetstatus.h \
    ../qtweetretweettome.h \
    ../qtweetretweetsofme.h \
    ../qtweetretweetbyme.h \
    ../qtweetnetbase.h \
    ../qtweetmentions.h \
    ../qtweetlib_global.h \
    ../qtweethometimeline.h \
    ../qtweetfriendstimeline.h \
    ../qtweetdirectmessages.h \
    ../oauthtwitter.h \
    ../oauth.h


FORMS    += mainwindow.ui
