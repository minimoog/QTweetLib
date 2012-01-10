/* Copyright (c) 2010, Antonie Jovanoski
 *
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact e-mail: Antonie Jovanoski <minimoog77_at_gmail.com>
 */

#include "qtweetconvert.h"
#include "qtweetstatus.h"
#include "qtweetdmstatus.h"
#include "qtweetuser.h"
#include "qtweetlist.h"
#include "qtweetplace.h"
#include "qtweetsearchresult.h"
#include "qtweetsearchpageresults.h"
#include "qtweetentityurl.h"
#include "qtweetentityhashtag.h"
#include "qtweetentityusermentions.h"
#include "cJSON.h"

/**
 *  Converts list of statuses
 */
QList<QTweetStatus> QTweetConvert::variantToStatusList(const QVariant &fromParser)
{
    QList<QTweetStatus> statuses;

    QList<QVariant> listStatus = fromParser.toList();

    foreach (const QVariant& status, listStatus) {
        QVariantMap statusMap = status.toMap();

        QTweetStatus tweetStatus = variantMapToStatus(statusMap);

        statuses.append(tweetStatus);
    }
    return statuses;
}

QList<QTweetStatus> QTweetConvert::cJSONToStatusList(cJSON *root)
{
    QList<QTweetStatus> statuses;

    if (root->type == cJSON_Array) {
        int size = cJSON_GetArraySize(root);

        for (int i = 0; i < size; ++i){
            // ### TODO
        }
    }

    return statuses;
}

/**
 *  Converts status
 */
QTweetStatus QTweetConvert::variantMapToStatus(const QVariantMap &var)
{
    QTweetStatus status;

    status.setCreatedAt(var["created_at"].toString());
    status.setText(var["text"].toString());
    status.setId(var["id"].toLongLong());
    status.setInReplyToUserId(var["in_reply_to_user_id"].toLongLong());
    status.setInReplyToScreenName(var["in_reply_to_screen_name"].toString());
    status.setFavorited(var["favorited"].toBool());

    QVariantMap userMap = var["user"].toMap();
    QTweetUser user = variantMapToUserInfo(userMap);

    status.setUser(user);
    status.setSource(var["source"].toString());
    status.setInReplyToStatusId(var["in_reply_to_status_id"].toLongLong());

    //check if contains native retweet
    if (var.contains("retweeted_status")) {
        QVariantMap retweetMap = var["retweeted_status"].toMap();

        QTweetStatus rtStatus = variantMapToStatus(retweetMap);

        status.setRetweetedStatus(rtStatus);
    }

    //parse place id if it's not null
    QVariant placeVar = var["place"];
    if (!placeVar.isNull()) {
        QTweetPlace place = variantMapToPlace(placeVar.toMap());
        status.setPlace(place);
    }

    //check if contains entities
    if (var.contains("entities")) {
        QVariantMap entitiesVarMap = var["entities"].toMap();

        //url entities
        QVariantList urlEntitiesVarList = entitiesVarMap["urls"].toList();

        foreach (const QVariant& urlEntityVar, urlEntitiesVarList) {
            QVariantMap urlEntityVarMap = urlEntityVar.toMap();
            QTweetEntityUrl urlEntity = variantMapToEntityUrl(urlEntityVarMap);

            status.addUrlEntity(urlEntity);
        }

        //hashtag entities
        QVariantList hashtagEntitiesVarList = entitiesVarMap["hashtags"].toList();

        foreach (const QVariant& hashtagEntityVar, hashtagEntitiesVarList) {
            QVariantMap hashtagEntityVarMap = hashtagEntityVar.toMap();
            QTweetEntityHashtag hashtagEntity = variantMapToEntityHashtag(hashtagEntityVarMap);

            status.addHashtagEntity(hashtagEntity);
        }

        //user mentions
        QVariantList userMentionsEntitiesVarList = entitiesVarMap["user_mentions"].toList();

        foreach (const QVariant& userMentionsEntityVar, userMentionsEntitiesVarList) {
            QVariantMap userMentionsEntityVarMap = userMentionsEntityVar.toMap();
            QTweetEntityUserMentions userMentionsEntity = variantMapToEntityUserMentions(userMentionsEntityVarMap);

            status.addUserMentionsEntity(userMentionsEntity);
        }
    }

    return status;
}

QTweetStatus QTweetConvert::cJSONToStatus(cJSON *root)
{
    QTweetStatus status;

    if (root->type == cJSON_Object) {
        status.setCreatedAt(cJSON_GetObjectItem(root, "created_at")->valuestring);
        status.setText(cJSON_GetObjectItem(root, "text")->valuestring);
        status.setId((qint64)cJSON_GetObjectItem(root, "id")->valuedouble);
        status.setInReplyToUserId((qint64)cJSON_GetObjectItem(root, "in_reply_to_user_id")->valuedouble);
        status.setInReplyToScreenName(cJSON_GetObjectItem(root, "in_reply_to_screen_name")->valuestring);
        status.setFavorited(cJSON_GetObjectItem(root, "favorited")->valuestring);   //### TODO

        cJSON *userObject = cJSON_GetObjectItem(root, "user");

        QTweetUser user = cJSONToUser(userObject);

        status.setUser(user);
        status.setSource(cJSON_GetObjectItem(root, "source")->valuestring);
        status.setInReplyToStatusId((qint64)cJSON_GetObjectItem(root, "in_reply_to_status_id")->valuedouble);

        cJSON *rtObject = cJSON_GetObjectItem(root, "retweeted_status");
        if (rtObject) {
            QTweetStatus rtStatus = cJSONToStatus(rtObject);
            status.setRetweetedStatus(rtStatus);
        }

        cJSON *placeObject = cJSON_GetObjectItem(root, "place");

        if (placeObject->type != cJSON_NULL) {
            // ### TODO Parse place object
        }

        cJSON *entObject = cJSON_GetObjectItem(root, "entities");
        if (entObject) {
            cJSON *urlArray = cJSON_GetObjectItem(entObject, "urls");

            for (int i = 0; i < cJSON_GetArraySize(urlArray); ++i) {
                cJSON *urlObject = cJSON_GetArrayItem(urlArray, i);
                // ### TODO url map
            }

            cJSON *hashtagsArray = cJSON_GetObjectItem(entObject, "hashtags");

            for (int i = 0; i < cJSON_GetArraySize(hashtagsArray); ++i) {
                cJSON *hashtagObject = cJSON_GetArrayItem(hashtagsArray, i);
                // ### TODO hashtag map
            }

            cJSON *userMentionsArray = cJSON_GetObjectItem(entObject, "user_mentions");

            for (int i = 0; i < cJSON_GetArraySize(userMentionsArray); ++i) {
                cJSON *userMentionObject =cJSON_GetArrayItem(userMentionsArray, i);
                // ### TODO usermentions map
            }
        }
    }

    return status;
}

/**
 *  Converts user info
 */
QTweetUser QTweetConvert::variantMapToUserInfo(const QVariantMap &var)
{
    QTweetUser userInfo;

    userInfo.setId(var["id"].toLongLong());

    //don't fill rest of it, when user info is trimmed
    if (var.contains("name")) {

        userInfo.setName(var["name"].toString());
        userInfo.setLocation(var["location"].toString());
        userInfo.setprofileImageUrl(var["profile_image_url"].toString());
        userInfo.setCreatedAt(var["created_at"].toString());
        userInfo.setFavouritesCount(var["favourites_count"].toInt());
        userInfo.setUrl(var["url"].toString());
        userInfo.setUtcOffset(var["utc_offset"].toInt());
        userInfo.setProtected(var["protected"].toBool());
        userInfo.setFollowersCount(var["followers_count"].toInt());
        userInfo.setVerified(var["verified"].toBool());
        userInfo.setGeoEnabled(var["geo_enabled"].toBool());
        userInfo.setDescription(var["description"].toString());
        userInfo.setTimezone(var["time_zone"].toString());
        userInfo.setFriendsCount(var["friends_count"].toInt());
        userInfo.setStatusesCount(var["statuses_count"].toInt());
        userInfo.setScreenName(var["screen_name"].toString());
        userInfo.setContributorsEnabled(var["contributors_enabled"].toBool());
        userInfo.setListedCount(var["listed_count"].toInt());
        userInfo.setLang(var["lang"].toString());

        //check if contains status
        if (var.contains("status")) {
            QVariantMap statusMap = var["status"].toMap();

            QTweetStatus status = variantMapToStatus(statusMap);

            userInfo.setStatus(status);
        }
    }

    return userInfo;
}

QTweetUser QTweetConvert::cJSONToUser(cJSON *root)
{
    QTweetUser user;

    user.setId((qint64)cJSON_GetObjectItem(root, "id")->valuedouble);

    cJSON *nameObject = cJSON_GetObjectItem(root, "name");

    if (nameObject) {
        user.setName(cJSON_GetObjectItem(nameObject, "name")->valuestring);
        user.setLocation(cJSON_GetObjectItem(nameObject, "location")->valuestring);
        user.setprofileImageUrl(cJSON_GetObjectItem(nameObject, "profile_image_url")->valuestring);
        user.setCreatedAt(cJSON_GetObjectItem(nameObject, "created_at")->valuestring);
        user.setFavouritesCount(cJSON_GetObjectItem(nameObject, "favourites_count")->valueint);
        user.setUrl(cJSON_GetObjectItem(nameObject, "url")->valuestring);
        user.setUtcOffset(cJSON_GetObjectItem(nameObject, "utc_offset")->valueint);
        user.setProtected(cJSON_GetObjectItem(nameObject, "protected")->valuestring);
        user.setFollowersCount(cJSON_GetObjectItem(nameObject, "followers_count")->valueint);
        user.setVerified(cJSON_GetObjectItem(nameObject, "verified")->valuestring);
        user.setGeoEnabled(cJSON_GetObjectItem(nameObject, "geo_enabled")->valuestring);
        user.setDescription(cJSON_GetObjectItem(nameObject, "description")->valuestring);
        user.setTimezone(cJSON_GetObjectItem(nameObject, "time_zone")->valuestring);
        user.setFriendsCount(cJSON_GetObjectItem(nameObject, "friends_count")->valueint);
        user.setStatusesCount(cJSON_GetObjectItem(nameObject, "statuses_count")->valueint);
        user.setScreenName(cJSON_GetObjectItem(nameObject, "screen_name")->valuestring);
        user.setContributorsEnabled(cJSON_GetObjectItem(nameObject, "contributors_enabled")->valuestring);
        user.setListedCount(cJSON_GetObjectItem(nameObject, "listed_count")->valueint);
        user.setLang(cJSON_GetObjectItem(nameObject, "lang")->valuestring);

        cJSON *statusObject = cJSON_GetObjectItem(root, "status");

        if (statusObject) {
            // ### TODO
        }
    }
}

/**
 *  Converts list of direct messages
 */
QList<QTweetDMStatus> QTweetConvert::variantToDirectMessagesList(const QVariant& fromParser)
{
    QList<QTweetDMStatus> directMessages;

    QList<QVariant> listMessages = fromParser.toList();

    foreach (const QVariant& message, listMessages) {
        QTweetDMStatus dmStatus = variantMapToDirectMessage(message.toMap());
        directMessages.append(dmStatus);
    }

    return directMessages;
}

QList<QTweetDMStatus> QTweetConvert::cJSONToDirectMessagesList(cJSON *root)
{
    QList<QTweetDMStatus> directMessages;

    if (root->type == cJSON_Array) {
        int size = cJSON_GetArraySize(root);

        for (int i = 0; i < size; i++) {
            // ### TODO: cJSONtoDirectMessage
        }
    }

    return directMessages;
}

/**
 *  Converts direct message
 */
QTweetDMStatus QTweetConvert::variantMapToDirectMessage(const QVariantMap& var)
{
    QTweetDMStatus directMessage;

    directMessage.setCreatedAt(var["created_at"].toString());
    directMessage.setSenderScreenName(var["sender_screen_name"].toString());

    QVariantMap senderMap = var["sender"].toMap();
    QTweetUser sender = variantMapToUserInfo(senderMap);

    directMessage.setSender(sender);

    directMessage.setText(var["text"].toString());
    directMessage.setRecipientScreenName(var["recipient_screen_name"].toString());
    directMessage.setId(var["id"].toLongLong());

    QVariantMap recipientMap = var["recipient"].toMap();
    QTweetUser recipient = variantMapToUserInfo(recipientMap);

    directMessage.setRecipient(recipient);

    directMessage.setRecipientId(var["recipient_id"].toLongLong());
    directMessage.setSenderId(var["sender_id"].toLongLong());

    return directMessage;
}

QTweetDMStatus QTweetConvert::cJSONToDirectMessage(cJSON *root)
{
    QTweetDMStatus directMessage;

    if (root->type == cJSON_Object) {
        directMessage.setCreatedAt(cJSON_GetObjectItem(root, "created_at")->valuestring);
        directMessage.setSenderScreenName(cJSON_GetObjectItem(root, "sender_screen_name")->valuestring);

        cJSON *senderObject = cJSON_GetObjectItem(root, "sender");
        // ### TODO:  cJSONToUser

        directMessage.setText(cJSON_GetObjectItem(root, "text")->valuestring);
        directMessage.setRecipientScreenName(cJSON_GetObjectItem(root, "recipient_screen_name")->valuestring);
        directMessage.setId((qint64)cJSON_GetObjectItem(root, "id")->valuedouble);

        cJSON *recipientObject = cJSON_GetObjectItem(root, "recipient");
        // ### TODO: cJSONToUser

        directMessage.setRecipientId((qint64)cJSON_GetObjectItem(root, "recipient_id")->valuedouble);
        directMessage.setSenderId((qint64)cJSON_GetObjectItem(root, "sender_id")->valuedouble);
    }

    return directMessage;
}

/**
 *  Converts tweet list
 */
QTweetList QTweetConvert::variantMapToTweetList(const QVariantMap& var)
{
    QTweetList list;

    list.setMode(var["mode"].toString());
    list.setDescription(var["description"].toString());
    list.setFollowing(var["following"].toBool());
    list.setMemberCount(var["member_count"].toInt());
    list.setFullName(var["full_name"].toString());
    list.setSubscriberCount(var["subscriber_count"].toInt());
    list.setSlug(var["slug"].toString());
    list.setName(var["name"].toString());
    list.setId(var["id"].toLongLong());
    list.setUri(var["uri"].toString());

    if (var.contains("user")) {
        QVariantMap userMap = var["user"].toMap();

        QTweetUser user = variantMapToUserInfo(userMap);

        list.setUser(user);
    }
    return list;
}

QTweetList QTweetConvert::cJSONToTweetList(cJSON *root)
{
    QTweetList list;

    if (root->type == cJSON_Object) {
        list.setMode(cJSON_GetObjectItem(root, "mode")->valuestring);
        list.setDescription(cJSON_GetObjectItem(root, "description")->valuestring);
        list.setFollowing(cJSON_GetObjectItem(root, "following")->valueint);
        list.setMemberCount(cJSON_GetObjectItem(root, "member_count")->valueint);
        list.setFullName(cJSON_GetObjectItem(root, "full_name")->valuestring);
        list.setSubscriberCount(cJSON_GetObjectItem(root, "subscriber_count")->valueint);
        list.setSlug(cJSON_GetObjectItem(root, "slug")->valuestring);
        list.setName(cJSON_GetObjectItem(root, "name")->valuestring);
        list.setId((qint64)cJSON_GetObjectItem(root, "id")->valuedouble);
        list.setUri(cJSON_GetObjectItem(root, "uri")->valuestring);

        cJSON *userObject = cJSON_GetObjectItem(root, "user");

        if (userObject) {
            // ### TODO cJSONToUser
        }
    }

    return list;
}

/**
 *  Converts list of user infos
 */
QList<QTweetUser> QTweetConvert::variantToUserInfoList(const QVariant& fromParser)
{
    QList<QTweetUser> users;

    QList<QVariant> listUsers = fromParser.toList();

    foreach (const QVariant& user, listUsers) {
        QTweetUser userInfo = variantMapToUserInfo(user.toMap());

        users.append(userInfo);
    }

    return users;
}

QList<QTweetUser> QTweetConvert::cJSONToUserInfoList(cJSON *root)
{
    QList<QTweetUser> users;

    if (root->type == cJSON_Array) {
        int size = cJSON_GetArraySize(root);

        for (int i = 0; i < size; i++) {
            cJSON *userObject = cJSON_GetArrayItem(root, i);

            // ### TODO cJSONToUser
        }
    }

    return users;
}

/**
 *  Converts list of tweet lists
 */
QList<QTweetList> QTweetConvert::variantToTweetLists(const QVariant& var)
{
    QList<QTweetList> lists;

    QList<QVariant> varLists = var.toList();

    foreach (const QVariant& varlist, varLists) {
        QTweetList tweetlist = variantMapToTweetList(varlist.toMap());

        lists.append(tweetlist);
    }

    return lists;
}

QList<QTweetList> QTweetConvert::cJSONToTweetLists(cJSON *root)
{
    QList<QTweetList> lists;

    if (root->type == cJSON_Array) {
        int size = cJSON_GetArraySize(root);

        for (int i = 0; i < size; i++) {
            cJSON *listObject = cJSON_GetArrayItem(root, i);

            QTweetList tweetList = cJSONToTweetList(listObject);
            lists.append(tweetList);
        }
    }

    return lists;
}

/**
 *  Converts search result
 */
QTweetSearchResult QTweetConvert::variantMapToSearchResult(const QVariantMap& var)
{
    QTweetSearchResult result;

    result.setCreatedAt(var["created_at"].toString());
    result.setFromUser(var["from_user"].toString());
    result.setId(var["id"].toLongLong());
    result.setLang(var["iso_language_code"].toString());
    result.setProfileImageUrl(var["profile_image_url"].toString());
    result.setSource(var["source"].toString());
    result.setText(var["text"].toString());
    result.setToUser(var["to_user"].toString());

    return result;
}

QTweetSearchResult QTweetConvert::cJSONToSearchResult(cJSON *root)
{
    QTweetSearchResult result;

    if (root->type == cJSON_Object) {
        result.setCreatedAt(cJSON_GetObjectItem(root, "created_at")->valuestring);
        result.setFromUser(cJSON_GetObjectItem(root, "from_user")->valuestring);
        result.setId((qint64)cJSON_GetObjectItem(root, "id")->valuedouble);
        result.setLang(cJSON_GetObjectItem(root, "iso_language_code")->valuestring);
        result.setProfileImageUrl(cJSON_GetObjectItem(root, "profile_image_url")->valuestring);
        result.setSource(cJSON_GetObjectItem(root, "source")->valuestring);
        result.setText(cJSON_GetObjectItem(root, "text")->valuestring);
        result.setToUser(cJSON_GetObjectItem(root, "to_user")->valuestring);
    }

    return result;
}

/**
 *  Converts page results
 */
QTweetSearchPageResults QTweetConvert::variantToSearchPageResults(const QVariant& var)
{
    QTweetSearchPageResults page;

    QVariantMap varMap = var.toMap();

    page.setMaxId(varMap["max_id"].toLongLong());
    page.setNextPage(varMap["next_page"].toByteArray());
    page.setPage(varMap["page"].toInt());
    page.setQuery(varMap["query"].toByteArray());
    page.setRefreshUrl(varMap["refresh_url"].toByteArray());
    page.setResultsPerPage(varMap["results_per_page"].toInt());
    page.setSinceId(varMap["since_id"].toLongLong());
    page.setTotal(varMap["total"].toInt());

    QList<QTweetSearchResult> resultList;
    QList<QVariant> resultVarList = varMap["results"].toList();

    foreach (const QVariant& resultVar, resultVarList) {
        QTweetSearchResult result = variantMapToSearchResult(resultVar.toMap());
        resultList.append(result);
    }

    page.setResults(resultList);

    return page;
}

QTweetSearchPageResults QTweetConvert::cJSONToSearchPageResults(cJSON *root)
{
    QTweetSearchPageResults page;

    if (root->type == cJSON_Object) {
        page.setMaxId((qint64)cJSON_GetObjectItem(root, "max_id")->valuedouble);
        page.setNextPage(cJSON_GetObjectItem(root, "next_page")->valuestring);  //?
        page.setPage(cJSON_GetObjectItem(root, "page")->valueint);
        page.setQuery(cJSON_GetObjectItem(root, "query")->valuestring);
        page.setRefreshUrl(cJSON_GetObjectItem(root, "refresh_url")->valuestring);
        page.setResultsPerPage(cJSON_GetObjectItem(root, "results_per_page")->valueint);
        page.setSinceId((qint64)cJSON_GetObjectItem(root, "since_id")->valuedouble);

        QList<QTweetSearchResult> resultList;

        cJSON *resultListObject = cJSON_GetObjectItem(root, "results");

        if (resultListObject) {
            int size = cJSON_GetArraySize(resultListObject);

            for (int i = 0; i < size; i++) {
                cJSON *resultObject = cJSON_GetArrayItem(resultListObject, i);
                QTweetSearchResult result = cJSONToSearchResult(resultObject);
                resultList.append(result);
            }
        }

        page.setResults(resultList);
    }

    return page;
}

/**
 *  Converts place
 */
QTweetPlace QTweetConvert::variantMapToPlace(const QVariantMap& var)
{
    QTweetPlace place;

    place.setName(var["name"].toString());
    place.setCountryCode(var["country_code"].toString());
    place.setCountry(var["country"].toString());
    place.setID(var["id"].toString());
    place.setFullName(var["full_name"].toString());

    QString placeType = var["place_type"].toString();

    if (placeType == "poi")
        place.setType(QTweetPlace::Poi);
    else if (placeType == "neighborhood")
        place.setType(QTweetPlace::Neighborhood);
    else if (placeType == "city")
        place.setType(QTweetPlace::City);
    else if (placeType == "admin")
        place.setType(QTweetPlace::Admin);
    else if (placeType == "country")
        place.setType(QTweetPlace::Country);
    else
        place.setType(QTweetPlace::Neighborhood);   //twitter default

    QVariant bbVar = var["bounding_box"];

    if (!bbVar.isNull()) {
        QVariantMap bbMap = bbVar.toMap();

        if (bbMap["type"].toString() == "Polygon") {
            QVariantList coordList = bbMap["coordinates"].toList();

            if (coordList.count() == 1) {
                QVariantList latLongList = coordList.at(0).toList();

                if (latLongList.count() == 4) {
                    QTweetGeoBoundingBox box;

                    QVariantList coordsBottomLeft = latLongList.at(0).toList();
                    box.setBottomLeft(QTweetGeoCoord(coordsBottomLeft.at(1).toDouble(), coordsBottomLeft.at(0).toDouble()));

                    QVariantList coordsBottomRight = latLongList.at(1).toList();
                    box.setBottomRight(QTweetGeoCoord(coordsBottomRight.at(1).toDouble(), coordsBottomRight.at(0).toDouble()));

                    QVariantList coordsTopRight = latLongList.at(2).toList();
                    box.setTopRight(QTweetGeoCoord(coordsTopRight.at(1).toDouble(), coordsTopRight.at(0).toDouble()));

                    QVariantList coordsTopLeft = latLongList.at(3).toList();
                    box.setTopLeft(QTweetGeoCoord(coordsTopLeft.at(1).toDouble(), coordsTopLeft.at(0).toDouble()));

                    place.setBoundingBox(box);
                }
            }
        }
    }
    return place;
}

QTweetPlace QTweetConvert::cJSONToPlace(cJSON *root)
{
    QTweetPlace place;

    if (root->type == cJSON_Object) {
        place.setName(cJSON_GetObjectItem(root, "name")->valuestring);
        place.setCountryCode(cJSON_GetObjectItem(root, "country_code")->valuestring);
        place.setCountry(cJSON_GetObjectItem(root, "country")->valuestring);
        place.setID(cJSON_GetObjectItem(root, "id")->valuestring);
        place.setFullName(cJSON_GetObjectItem(root, "full_name")->valuestring);

        QString placeType = QString(cJSON_GetObjectItem(root, "place_type")->valuestring);

        if (placeType == "poi")
            place.setType(QTweetPlace::Poi);
        else if (placeType == "neighborhood")
            place.setType(QTweetPlace::Neighborhood);
        else if (placeType == "city")
            place.setType(QTweetPlace::City);
        else if (placeType == "admin")
            place.setType(QTweetPlace::Admin);
        else if (placeType == "country")
            place.setType(QTweetPlace::Country);
        else
            place.setType(QTweetPlace::Neighborhood);   //twitter default

        cJSON *bboxObject = cJSON_GetObjectItem(root, "bounding_box");
        if (bboxObject) {
            QString type = QString(cJSON_GetObjectItem(bboxObject, "type")->valuestring);

            if (type == "Polygon") {
                cJSON *coordListObject = cJSON_GetObjectItem(bboxObject, "coordinates");

                if (coordListObject) {
                    cJSON *latLongObject = cJSON_GetArrayItem(coordListObject, 0);

                    if (latLongObject->type == cJSON_Array) {
                        QTweetGeoBoundingBox box;

                        cJSON *coordsBottomLeftObject = cJSON_GetArrayItem(latLongObject, 0);
                        cJSON *coordsBottomRightObject = cJSON_GetArrayItem(latLongObject, 1);
                        cJSON *coordsTopRightObject = cJSON_GetArrayItem(latLongObject, 2);
                        cJSON *coordsTopLeftObject = cJSON_GetArrayItem(latLongObject, 3);

                        box.setBottomLeft(QTweetGeoCoord(cJSON_GetArrayItem(coordsBottomLeftObject, 1)->valuedouble,
                                                         cJSON_GetArrayItem(coordsBottomLeftObject, 0)->valuedouble));
                        box.setBottomRight(QTweetGeoCoord(cJSON_GetArrayItem(coordsBottomRightObject, 1)->valuedouble,
                                                          cJSON_GetArrayItem(coordsBottomRightObject, 0)->valuedouble));
                        box.setTopRight(QTweetGeoCoord(cJSON_GetArrayItem(coordsTopRightObject, 1)->valuedouble,
                                                       cJSON_GetArrayItem(coordsTopRightObject, 0)->valuedouble));
                        box.setTopLeft(QTweetGeoCoord(cJSON_GetArrayItem(coordsTopLeftObject, 1)->valuedouble,
                                                      cJSON_GetArrayItem(coordsTopLeftObject, 0)->valuedouble));

                        place.setBoundingBox(box);
                    }
                }
            }
        }
    }

    return place;
}

//not to be used in timelines api, but in geo api, where place contains other places
//is it recursive responsive?
QTweetPlace QTweetConvert::variantMapToPlaceRecursive(const QVariantMap& var)
{
    QTweetPlace place;

    place.setName(var["name"].toString());
    place.setCountryCode(var["country_code"].toString());
    place.setCountry(var["country"].toString());
    place.setID(var["id"].toString());
    place.setFullName(var["full_name"].toString());

    QString placeType = var["place_type"].toString();

    if (placeType == "poi")
        place.setType(QTweetPlace::Poi);
    else if (placeType == "neighborhood")
        place.setType(QTweetPlace::Neighborhood);
    else if (placeType == "city")
        place.setType(QTweetPlace::City);
    else if (placeType == "admin")
        place.setType(QTweetPlace::Admin);
    else if (placeType == "country")
        place.setType(QTweetPlace::Country);
    else
        place.setType(QTweetPlace::Neighborhood);   //twitter default

    QVariant bbVar = var["bounding_box"];

    if (!bbVar.isNull()) {
        QVariantMap bbMap = bbVar.toMap();

        if (bbMap["type"].toString() == "Polygon") {
            QVariantList coordList = bbMap["coordinates"].toList();

            if (coordList.count() == 1) {
                QVariantList latLongList = coordList.at(0).toList();

                if (latLongList.count() == 4) {
                    QTweetGeoBoundingBox box;

                    QVariantList coordsBottomLeft = latLongList.at(0).toList();
                    box.setBottomLeft(QTweetGeoCoord(coordsBottomLeft.at(1).toDouble(), coordsBottomLeft.at(0).toDouble()));

                    QVariantList coordsBottomRight = latLongList.at(1).toList();
                    box.setBottomRight(QTweetGeoCoord(coordsBottomRight.at(1).toDouble(), coordsBottomRight.at(0).toDouble()));

                    QVariantList coordsTopRight = latLongList.at(2).toList();
                    box.setTopRight(QTweetGeoCoord(coordsTopRight.at(1).toDouble(), coordsTopRight.at(0).toDouble()));

                    QVariantList coordsTopLeft = latLongList.at(3).toList();
                    box.setTopLeft(QTweetGeoCoord(coordsTopLeft.at(1).toDouble(), coordsTopLeft.at(0).toDouble()));

                    place.setBoundingBox(box);
                }
            }
        }
    }

    QVariantList containedVarList = var["contained_within"].toList();

    QList<QTweetPlace> containedInPlacesList;

    if (!containedVarList.isEmpty()) {
        foreach (const QVariant& containedVar, containedVarList) {
            QVariantMap containedPlaceMap = containedVar.toMap();

            QTweetPlace containedPlace = variantMapToPlaceRecursive(containedPlaceMap);

            containedInPlacesList.append(containedPlace);
        }
    }

    place.setContainedWithin(containedInPlacesList);

    return place;
}

QTweetPlace QTweetConvert::cJSONToPlaceRecursive(cJSON *root)
{
    QTweetPlace place;

    if (root->type == cJSON_Object) {
        place.setName(cJSON_GetObjectItem(root, "name")->valuestring);
        place.setCountryCode(cJSON_GetObjectItem(root, "country_code")->valuestring);
        place.setCountry(cJSON_GetObjectItem(root, "country")->valuestring);
        place.setID(cJSON_GetObjectItem(root, "id")->valuestring);
        place.setFullName(cJSON_GetObjectItem(root, "full_name")->valuestring);

        QString placeType = QString(cJSON_GetObjectItem(root, "place_type")->valuestring);

        if (placeType == "poi")
            place.setType(QTweetPlace::Poi);
        else if (placeType == "neighborhood")
            place.setType(QTweetPlace::Neighborhood);
        else if (placeType == "city")
            place.setType(QTweetPlace::City);
        else if (placeType == "admin")
            place.setType(QTweetPlace::Admin);
        else if (placeType == "country")
            place.setType(QTweetPlace::Country);
        else
            place.setType(QTweetPlace::Neighborhood);   //twitter default

        cJSON *bboxObject = cJSON_GetObjectItem(root, "bounding_box");
        if (bboxObject) {
            QString type = QString(cJSON_GetObjectItem(bboxObject, "type")->valuestring);

            if (type == "Polygon") {
                cJSON *coordListObject = cJSON_GetObjectItem(bboxObject, "coordinates");

                if (coordListObject) {
                    cJSON *latLongObject = cJSON_GetArrayItem(coordListObject, 0);

                    if (latLongObject->type == cJSON_Array) {
                        QTweetGeoBoundingBox box;

                        cJSON *coordsBottomLeftObject = cJSON_GetArrayItem(latLongObject, 0);
                        cJSON *coordsBottomRightObject = cJSON_GetArrayItem(latLongObject, 1);
                        cJSON *coordsTopRightObject = cJSON_GetArrayItem(latLongObject, 2);
                        cJSON *coordsTopLeftObject = cJSON_GetArrayItem(latLongObject, 3);

                        box.setBottomLeft(QTweetGeoCoord(cJSON_GetArrayItem(coordsBottomLeftObject, 1)->valuedouble,
                                                         cJSON_GetArrayItem(coordsBottomLeftObject, 0)->valuedouble));
                        box.setBottomRight(QTweetGeoCoord(cJSON_GetArrayItem(coordsBottomRightObject, 1)->valuedouble,
                                                          cJSON_GetArrayItem(coordsBottomRightObject, 0)->valuedouble));
                        box.setTopRight(QTweetGeoCoord(cJSON_GetArrayItem(coordsTopRightObject, 1)->valuedouble,
                                                       cJSON_GetArrayItem(coordsTopRightObject, 0)->valuedouble));
                        box.setTopLeft(QTweetGeoCoord(cJSON_GetArrayItem(coordsTopLeftObject, 1)->valuedouble,
                                                      cJSON_GetArrayItem(coordsTopLeftObject, 0)->valuedouble));

                        place.setBoundingBox(box);
                    }
                }
            }
        }
    }

    cJSON *containedObject = cJSON_GetObjectItem(root, "contained_within");

    QList<QTweetPlace> containedInPlacesList;

    if (containedObject) {
        int size = cJSON_GetArraySize(containedObject);

        for (int i = 0; i < size; i++) {
            cJSON *containedPlaceObject = cJSON_GetArrayItem(containedObject, i);
            QTweetPlace containedPlace = cJSONToPlaceRecursive(containedPlaceObject);
            containedInPlacesList.append(containedPlace);
        }
    }

    place.setContainedWithin(containedInPlacesList);

    return place;
}

/**
 *  Convers list of places
 */
QList<QTweetPlace> QTweetConvert::variantToPlaceList(const QVariant& fromParser)
{
    QList<QTweetPlace> placeList;

    QVariantMap responseVarMap = fromParser.toMap();
    QVariantMap resultVarMap = responseVarMap["result"].toMap();
    QVariantList placesVarList = resultVarMap["places"].toList();

    foreach (const QVariant& placeVar, placesVarList) {
        QTweetPlace place = variantMapToPlaceRecursive(placeVar.toMap());
        placeList.append(place);
    }

    return placeList;
}

QList<QTweetPlace> QTweetConvert::cJSONToPlaceList(cJSON *root)
{
    QList<QTweetPlace> placeList;

    cJSON *resultObject = cJSON_GetObjectItem(root, "result");
    cJSON *placesObject = cJSON_GetObjectItem(resultObject, "places");

    if (placesObject) {
        int size = cJSON_GetArraySize(placesObject);

        for (int i = 0; i < size; i++) {
            cJSON *placeObject = cJSON_GetArrayItem(placesObject, i);
            QTweetPlace place = cJSONToPlaceRecursive(placeObject);
            placeList.append(place);
        }
    }

    return placeList;
}

QTweetEntityUrl QTweetConvert::variantMapToEntityUrl(const QVariantMap &var)
{
    QString url = var["url"].toString();
    QString displayUrl = var["display_url"].toString();
    QString expandedUrl = var["expanded_url"].toString();

    QTweetEntityUrl urlEntity;
    urlEntity.setUrl(url);
    urlEntity.setDisplayUrl(displayUrl);
    urlEntity.setExpandedUrl(expandedUrl);

    return urlEntity;
}

QTweetEntityHashtag QTweetConvert::variantMapToEntityHashtag(const QVariantMap &var)
{
    QTweetEntityHashtag hashtagEntity;
    hashtagEntity.setText(var["text"].toString());

    return hashtagEntity;
}

QTweetEntityUserMentions QTweetConvert::variantMapToEntityUserMentions(const QVariantMap &var)
{
    QTweetEntityUserMentions userMentionsEntity;
    userMentionsEntity.setScreenName(var["screen_name"].toString());
    userMentionsEntity.setName(var["name"].toString());
    userMentionsEntity.setUserid(var["id"].toLongLong());

    return userMentionsEntity;
}
