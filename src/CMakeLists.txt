SET(QTWEETLIB_SOURCES
    oauth.cpp
    oauthtwitter.cpp
    qtweetaccountratelimitstatus.cpp
    qtweetaccountverifycredentials.cpp
    qtweetblocksblocking.cpp
    qtweetblocksblockingids.cpp
    qtweetblockscreate.cpp
    qtweetblocksdestroy.cpp
    qtweetblocksexists.cpp
    qtweetconvert.cpp
    qtweetdirectmessagedestroy.cpp
    qtweetdirectmessagenew.cpp
    qtweetdirectmessages.cpp
    qtweetdirectmessagessent.cpp
    qtweetdmstatus.cpp
    qtweetentityhashtag.cpp
    qtweetentitymedia.cpp
    qtweetentityurl.cpp
    qtweetentityusermentions.cpp
    qtweetfavorites.cpp
    qtweetfavoritescreate.cpp
    qtweetfavoritesdestroy.cpp
    qtweetfollowersid.cpp
    qtweetfriendshipcreate.cpp
    qtweetfriendshipdestroy.cpp
    qtweetfriendsid.cpp
    qtweetgeoboundingbox.cpp
    qtweetgeocoord.cpp
    qtweetgeoplacecreate.cpp
    qtweetgeoplaceid.cpp
    qtweetgeoreversegeocode.cpp
    qtweetgeosearch.cpp
    qtweetgeosimilarplaces.cpp
    qtweethometimeline.cpp
    qtweetlistaddmember.cpp
    qtweetlist.cpp
    qtweetlistcreate.cpp
    qtweetlistdeletemember.cpp
    qtweetlistshowlist.cpp
    qtweetlistsubscribe.cpp
    qtweetlistupdate.cpp
    qtweetmentions.cpp
    qtweetnetbase.cpp
    qtweetplace.cpp
    qtweetsearch.cpp
    qtweetsearchpageresults.cpp
    qtweetsearchresult.cpp
    qtweetstatus.cpp
    qtweetstatusdestroy.cpp
    qtweetstatusretweetbyid.cpp
    qtweetstatusretweet.cpp
    qtweetstatusretweetedby.cpp
    qtweetstatusretweets.cpp
    qtweetstatusshow.cpp
    qtweetstatusupdate.cpp
    qtweetuser.cpp
    qtweetuserlookup.cpp
    qtweetusersearch.cpp
    qtweetusershow.cpp
    qtweetuserstatusesfollowers.cpp
    qtweetuserstream.cpp
    qtweetusertimeline.cpp
)

SET(QTWEETLIB_MOC_HEADERS
    oauth.h
    oauthtwitter.h
    qtweetaccountratelimitstatus.h
    qtweetaccountverifycredentials.h
    qtweetblocksblocking.h
    qtweetblocksblockingids.h
    qtweetblockscreate.h
    qtweetblocksdestroy.h
    qtweetblocksexists.h
    qtweetdirectmessagedestroy.h
    qtweetdirectmessagenew.h
    qtweetdirectmessages.h
    qtweetdirectmessagessent.h
    qtweetfavorites.h
    qtweetfavoritescreate.h
    qtweetfavoritesdestroy.h
    qtweetfollowersid.h
    qtweetfriendshipcreate.h
    qtweetfriendshipdestroy.h
    qtweetfriendsid.h
    qtweetgeoplacecreate.h
    qtweetgeoplaceid.h
    qtweetgeoreversegeocode.h
    qtweetgeosearch.h
    qtweetgeosimilarplaces.h
    qtweethometimeline.h
    qtweetlistaddmember.h
    qtweetlistcreate.h
    qtweetlistdeletemember.h
    qtweetlistshowlist.h
    qtweetlistsubscribe.h
    qtweetlistupdate.h
    qtweetmentions.h
    qtweetnetbase.h
    qtweetsearch.h
    qtweetstatusdestroy.h
    qtweetstatusretweetbyid.h
    qtweetstatusretweet.h
    qtweetstatusretweetedby.h
    qtweetstatusretweets.h
    qtweetstatusshow.h
    qtweetstatusupdate.h
    qtweetuserlookup.h
    qtweetusersearch.h
    qtweetusershow.h
    qtweetuserstatusesfollowers.h
    qtweetuserstream.h
    qtweetusertimeline.h
)

SET(QTWEETLIB_HEADERS
    ${QTWEETLIB_MOC_HEADERS}
    qtweetlib_global.h
    qtweetconvert.h
    qtweetdmstatus.h
    qtweetentityhashtag.h
    qtweetentitymedia.h
    qtweetentityurl.h
    qtweetentityusermentions.h
    qtweetgeoboundingbox.h
    qtweetgeocoord.h
    qtweetlist.h
    qtweetplace.h
    qtweetsearchpageresults.h
    qtweetsearchresult.h
    qtweetstatus.h
    qtweetuser.h
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
