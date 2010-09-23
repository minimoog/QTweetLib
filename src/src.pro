QT       += network
QT       -= gui

TEMPLATE = lib
TARGET = QTweetLib
DESTDIR = ../lib
LIBS += ../lib/qjson.lib
CONFIG += create_prl
INCLUDEPATH += ..

windows: {
	DEFINES += QTWEETLIB_MAKEDLL
}

HEADERS += \
    oauth.h \
    signalwaiter.h \
    oauthtwitter.h \
    qtweetfriendstimeline.h \
    qtweetlib_global.h \
    qtweethometimeline.h \
    qtweetmentions.h \
    qtweetnetbase.h \
    qtweetretweetbyme.h \
    qtweetretweetsofme.h \
    qtweetretweettome.h \
    qtweetstatus.h \
    qtweetstatusshow.h \
    qtweetstatusupdate.h \
    qtweetuser.h \
    qtweetusertimeline.h \
    qtweetstatusdestroy.h \
    qtweetstatusretweet.h \
    qtweetstatusretweets.h \
    qtweetusershow.h \
    qtweetuserlookup.h \
    qtweetdirectmessages.h \
    qtweetuserstream.h \
    qtweetdmstatus.h \
    qtweetusersearch.h \
    qtweetuserstatusesfriends.h \
    qtweetuserstatusesfollowers.h \
    qtweetlist.h \
    qtweetlistcreate.h

SOURCES += \
    oauth.cpp \
    signalwaiter.cpp \
    oauthtwitter.cpp \
    qtweetfriendstimeline.cpp \
    qtweethometimeline.cpp \
    qtweetmentions.cpp \
    qtweetnetbase.cpp \
    qtweetretweetbyme.cpp \
    qtweetretweetsofme.cpp \
    qtweetretweettome.cpp \
    qtweetstatus.cpp \
    qtweetstatusshow.cpp \
    qtweetstatusupdate.cpp \
    qtweetuser.cpp \
    qtweetusertimeline.cpp \
    qtweetstatusdestroy.cpp \
    qtweetstatusretweet.cpp \
    qtweetstatusretweets.cpp \
    qtweetusershow.cpp \
    qtweetuserlookup.cpp \
    qtweetdirectmessages.cpp \
    qtweetuserstream.cpp \
    qtweetdmstatus.cpp \
    qtweetusersearch.cpp \
    qtweetuserstatusesfriends.cpp \
    qtweetuserstatusesfollowers.cpp \
    qtweetlist.cpp \
    qtweetlistcreate.cpp
