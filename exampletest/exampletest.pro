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
    ../oauth.cpp \
    ../qtweetuserstream.cpp \
    ../qjson/serializerrunnable.cpp \
    ../qjson/serializer.cpp \
    ../qjson/qobjecthelper.cpp \
    ../qjson/parserrunnable.cpp \
    ../qjson/parser.cpp \
    ../qjson/json_scanner.cpp \
    ../qjson/json_parser.cc

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
    ../oauth.h \
    ../qtweetuserstream.h \
    ../qjson/stack.hh \
    ../qjson/serializerrunnable.h \
    ../qjson/serializer.h \
    ../qjson/qobjecthelper.h \
    ../qjson/qjson_export.h \
    ../qjson/qjson_debug.h \
    ../qjson/position.hh \
    ../qjson/parserrunnable.h \
    ../qjson/parser_p.h \
    ../qjson/parser.h \
    ../qjson/location.hh \
    ../qjson/json_scanner.h \
    ../qjson/json_parser.hh


FORMS    += mainwindow.ui

OTHER_FILES += \
    ../qjson/json_parser.yy
