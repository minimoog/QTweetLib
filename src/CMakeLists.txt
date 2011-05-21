SET(QTWEETLIB_SOURCES
    oauth.cpp
    oauthtwitter.cpp
    qtweetfriendstimeline.cpp
    qtweethometimeline.cpp
    qtweetmentions.cpp
    qtweetnetbase.cpp
    qtweetretweetbyme.cpp
    qtweetretweetsofme.cpp
    qtweetretweettome.cpp
    qtweetstatus.cpp
    qtweetstatusshow.cpp
    qtweetstatusupdate.cpp
    qtweetuser.cpp
    qtweetusertimeline.cpp
    qtweetstatusdestroy.cpp
    qtweetstatusretweet.cpp
    qtweetstatusretweets.cpp
    qtweetusershow.cpp
    qtweetuserlookup.cpp
    qtweetdirectmessages.cpp
    qtweetuserstream.cpp
    qtweetdmstatus.cpp
    qtweetusersearch.cpp
    qtweetuserstatusesfriends.cpp
    qtweetuserstatusesfollowers.cpp
    qtweetlist.cpp
    qtweetlistcreate.cpp
    qtweetlistupdate.cpp
    qtweetlistgetlists.cpp
    qtweetlistshowlist.cpp
    qtweetlistdeletelist.cpp
    qtweetliststatuses.cpp
    qtweetlistmemberships.cpp
    qtweetlistsubscriptions.cpp
    qtweetlistgetmembers.cpp
    qtweetlistaddmember.cpp
    qtweetlistdeletemember.cpp
    qtweetlistsubscribers.cpp
    qtweetlistsubscribe.cpp
    qtweetlistunsubscribe.cpp
    qtweetdirectmessagessent.cpp
    qtweetdirectmessagenew.cpp
    qtweetdirectmessagedestroy.cpp
    qtweetfriendshipcreate.cpp
    qtweetfriendshipdestroy.cpp
    qtweetfriendsid.cpp
    qtweetfollowersid.cpp
    qtweetaccountverifycredentials.cpp
    qtweetaccountratelimitstatus.cpp
    qtweetfavorites.cpp
    qtweetfavoritescreate.cpp
    qtweetfavoritesdestroy.cpp
    qtweetsearch.cpp
    qtweetsearchresult.cpp
    qtweetsearchpageresults.cpp
    qtweetplace.cpp
    qtweetgeoreversegeocode.cpp
    qtweetgeosearch.cpp
    qtweetgeosimilarplaces.cpp
    qtweetgeoplaceid.cpp
    qtweetgeoplacecreate.cpp
    qtweetgeocoord.cpp
    qtweetgeoboundingbox.cpp
    qtweetconvert.cpp
    qtweetentityurl.cpp
    qtweetentityhashtag.cpp
    qtweetentityusermentions.cpp
    qtweetblocksblocking.cpp
    qtweetblocksblockingids.cpp
    qtweetblockscreate.cpp
    qtweetblocksdestroy.cpp
    qtweetblocksexists.cpp
)

SET(QTWEETLIB_MOC_HEADERS
    oauth.h
    oauthtwitter.h
    qtweetfriendstimeline.h
    qtweethometimeline.h
    qtweetmentions.h
    qtweetnetbase.h
    qtweetretweetbyme.h
    qtweetretweetsofme.h
    qtweetretweettome.h
    qtweetstatusshow.h
    qtweetstatusupdate.h
    qtweetusertimeline.h
    qtweetstatusdestroy.h
    qtweetstatusretweet.h
    qtweetstatusretweets.h
    qtweetusershow.h
    qtweetuserlookup.h
    qtweetdirectmessages.h
    qtweetuserstream.h
    qtweetusersearch.h
    qtweetuserstatusesfriends.h
    qtweetuserstatusesfollowers.h
    qtweetlistcreate.h
    qtweetlistupdate.h
    qtweetlistgetlists.h
    qtweetlistshowlist.h
    qtweetlistdeletelist.h
    qtweetliststatuses.h
    qtweetlistmemberships.h
    qtweetlistsubscriptions.h
    qtweetlistgetmembers.h
    qtweetlistaddmember.h
    qtweetlistdeletemember.h
    qtweetlistsubscribers.h
    qtweetlistsubscribe.h
    qtweetlistunsubscribe.h
    qtweetdirectmessagessent.h
    qtweetdirectmessagenew.h
    qtweetdirectmessagedestroy.h
    qtweetfriendshipcreate.h
    qtweetfriendshipdestroy.h
    qtweetfriendsid.h
    qtweetfollowersid.h
    qtweetaccountverifycredentials.h
    qtweetaccountratelimitstatus.h
    qtweetfavorites.h
    qtweetfavoritescreate.h
    qtweetfavoritesdestroy.h
    qtweetsearch.h
    qtweetgeoreversegeocode.h
    qtweetgeosearch.h
    qtweetgeosimilarplaces.h
    qtweetgeoplaceid.h
    qtweetgeoplacecreate.h
    qtweetblocksblocking.h
    qtweetblocksblockingids.h
    qtweetblockscreate.h
    qtweetblocksdestroy.h
    qtweetblocksexists.h
)

SET(QTWEETLIB_HEADERS
    ${QTWEETLIB_MOC_HEADERS}
    qtweetlib_global.h
    qtweetstatus.h
    qtweetuser.h
    qtweetdmstatus.h
    qtweetlist.h
    qtweetsearchresult.h
    qtweetsearchpageresults.h
    qtweetplace.h
    qtweetgeocoord.h
    qtweetgeoboundingbox.h
    qtweetconvert.h
    qtweetentityurl.h
    qtweetentityhashtag.h
    qtweetentityusermentions.h
)

INCLUDE_DIRECTORIES(
    .
    ${QT_INCLUDE_DIR}
    ${QT_INCLUDES}
    ${QJSON_INCLUDE_DIR}
)

QT4_WRAP_CPP( QTWEETLIB_MOC ${QTWEETLIB_MOC_HEADERS} )

ADD_LIBRARY( QTweetLib SHARED ${QTWEETLIB_SOURCES} ${QTWEETLIB_MOC} )

SET_TARGET_PROPERTIES( QTweetLib PROPERTIES
    VERSION ${QTWEETLIB_VERSION}
    SOVERSION ${QTWEETLIB_SONAME}
)

TARGET_LINK_LIBRARIES( QTweetLib
    ${QT_LIBRARIES}
    ${QJSON_LIBRARIES}
)

INSTALL( TARGETS QTweetLib
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib${LIB_SUFFIX}
    ARCHIVE DESTINATION lib${LIB_SUFFIX}
    BUNDLE DESTINATION library
)

INSTALL( FILES ${QTWEETLIB_HEADERS}
    DESTINATION include/QTweetLib
)
