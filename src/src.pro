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
    qtweetstatusupdate.h \
    qtweetuser.h \
    qtweetusertimeline.h \
    qtweetstatusesdestroy.h \
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
    qtweetentitymedia.h \
    qtweetstatusupdatewithmedia.h \
    qtweetdirectmessagesshow.h \
    qtweetretweetsofme.h \
    qtweetstatusesretweetsid.h \
    qtweetstatusesshowid.h \
    qtweetstatusesretweetid.h

SOURCES += \
    oauth.cpp \
    oauthtwitter.cpp \
    qtweethometimeline.cpp \
    qtweetmentions.cpp \
    qtweetnetbase.cpp \
    qtweetstatus.cpp \
    qtweetstatusupdate.cpp \
    qtweetuser.cpp \
    qtweetusertimeline.cpp \
    qtweetstatusesdestroy.cpp \
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
    qtweetentitymedia.cpp \
    qtweetstatusupdatewithmedia.cpp \
    qtweetdirectmessagesshow.cpp \
    qtweetretweetsofme.cpp \
    qtweetstatusesretweetsid.cpp \
    qtweetstatusesshowid.cpp \
    qtweetstatusesretweetid.cpp

OTHER_FILES +=






