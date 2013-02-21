QT       += network
QT       += gui

TEMPLATE = lib
TARGET = qtweetlib
DESTDIR = ../lib
CONFIG += create_prl
INCLUDEPATH += ..

windows: {
	DEFINES += QTWEETLIB_MAKEDLL
}

HEADERS += \
    oauth.h \
    oauthtwitter.h \
    qtweetlib_global.h \
    qtweethometimeline.h \
    qtweetmentions.h \
    qtweetnetbase.h \
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
    qtweetuserstatusesfollowers.h \
    qtweetlist.h \
    qtweetlistcreate.h \
    qtweetlistupdate.h \
    qtweetlistshowlist.h \
    qtweetlistaddmember.h \
    qtweetlistdeletemember.h \
    qtweetlistsubscribe.h \
    qtweetdirectmessagessent.h \
    qtweetdirectmessagenew.h \
    qtweetdirectmessagedestroy.h \
    qtweetfriendshipcreate.h \
    qtweetfriendshipdestroy.h \
    qtweetfriendsid.h \
    qtweetfollowersid.h \
    qtweetaccountverifycredentials.h \
    qtweetaccountratelimitstatus.h \
    qtweetfavorites.h \
    qtweetfavoritescreate.h \
    qtweetfavoritesdestroy.h \
    qtweetsearch.h \
    qtweetsearchresult.h \
    qtweetsearchpageresults.h \
    qtweetplace.h \
    qtweetgeoreversegeocode.h \
    qtweetgeosearch.h \
    qtweetgeosimilarplaces.h \
    qtweetgeoplaceid.h \
    qtweetgeoplacecreate.h \
    qtweetgeocoord.h \
    qtweetgeoboundingbox.h \
    qtweetconvert.h \
    qtweetentityurl.h \
    qtweetentityhashtag.h \
    qtweetentityusermentions.h \
    qtweetblockscreate.h \
    qtweetblocksdestroy.h \
    qtweetblocksexists.h \
    qtweetblocksblocking.h \
    qtweetblocksblockingids.h \
    qtweetstatusretweetedby.h \
    qtweetstatusretweetbyid.h \
    json/qjsonwriter_p.h \
    json/qjsonvalue.h \
    json/qjsonparser_p.h \
    json/qjsonobject.h \
    json/qjsonexport.h \
    json/qjsondocument.h \
    json/qjsonarray.h \
    json/qjson_p.h \
    qtweetentitymedia.h \
    qtweetstatusupdatewithmedia.h \
    qtweetdirectmessagesshow.h

SOURCES += \
    oauth.cpp \
    oauthtwitter.cpp \
    qtweethometimeline.cpp \
    qtweetmentions.cpp \
    qtweetnetbase.cpp \
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
    qtweetuserstatusesfollowers.cpp \
    qtweetlist.cpp \
    qtweetlistcreate.cpp \
    qtweetlistupdate.cpp \
    qtweetlistshowlist.cpp \
    qtweetlistaddmember.cpp \
    qtweetlistdeletemember.cpp \
    qtweetlistsubscribe.cpp \
    qtweetdirectmessagessent.cpp \
    qtweetdirectmessagenew.cpp \
    qtweetdirectmessagedestroy.cpp \
    qtweetfriendshipcreate.cpp \
    qtweetfriendshipdestroy.cpp \
    qtweetfriendsid.cpp \
    qtweetfollowersid.cpp \
    qtweetaccountverifycredentials.cpp \
    qtweetaccountratelimitstatus.cpp \
    qtweetfavorites.cpp \
    qtweetfavoritescreate.cpp \
    qtweetfavoritesdestroy.cpp \
    qtweetsearch.cpp \
    qtweetsearchresult.cpp \
    qtweetsearchpageresults.cpp \
    qtweetplace.cpp \
    qtweetgeoreversegeocode.cpp \
    qtweetgeosearch.cpp \
    qtweetgeosimilarplaces.cpp \
    qtweetgeoplaceid.cpp \
    qtweetgeoplacecreate.cpp \
    qtweetgeocoord.cpp \
    qtweetgeoboundingbox.cpp \
    qtweetconvert.cpp \
    qtweetentityurl.cpp \
    qtweetentityhashtag.cpp \
    qtweetentityusermentions.cpp \
    qtweetblockscreate.cpp \
    qtweetblocksdestroy.cpp \
    qtweetblocksexists.cpp \
    qtweetblocksblocking.cpp \
    qtweetblocksblockingids.cpp \
    qtweetstatusretweetedby.cpp \
    qtweetstatusretweetbyid.cpp \
    json/qjsonwriter.cpp \
    json/qjsonvalue.cpp \
    json/qjsonparser.cpp \
    json/qjsonobject.cpp \
    json/qjsondocument.cpp \
    json/qjsonarray.cpp \
    json/qjson.cpp \
    qtweetentitymedia.cpp \
    qtweetstatusupdatewithmedia.cpp \
    qtweetdirectmessagesshow.cpp

OTHER_FILES +=






