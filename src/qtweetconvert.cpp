/* Copyright 2010 Antonie Jovanoski
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Contact e-mail: Antonie Jovanoski <minimoog77_at_gmail.com>
 */

#include "qtweetconvert.h"
#include <QSize>
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
#include "qtweetentitymedia.h"
#include "json/qjsonarray.h"
#include "json/qjsonobject.h"

QList<QTweetStatus> QTweetConvert::jsonArrayToStatusList(const QJsonArray &jsonArray)
{
    QList<QTweetStatus> statuses;

    for (int i = 0; i < jsonArray.size(); ++ i) {
        QTweetStatus tweetStatus = jsonObjectToStatus(jsonArray[i].toObject());

        statuses.append(tweetStatus);
    }

    return statuses;
}

QTweetStatus QTweetConvert::jsonObjectToStatus(const QJsonObject& json)
{
    QTweetStatus status;

    status.setCreatedAt(json["created_at"].toString());
    status.setText(json["text"].toString());
    status.setId(static_cast<qint64>(json["id_str"].toString().toLong()));
    status.setInReplyToUserId(static_cast<qint64>(json["in_reply_to_user_id_str"].toString().toLong()));
    status.setInReplyToScreenName(json["in_reply_to_screen_name"].toString());
    status.setFavorited(json["favorited"].toBool());

    QJsonObject userObject = json["user"].toObject();
    QTweetUser user = jsonObjectToUser(userObject);
    status.setUser(user);

    status.setSource(json["source"].toString());
    status.setInReplyToStatusId(static_cast<qint64>(json["in_reply_to_status_id_str"].toString().toLong()));

    //check if contains native retweet
    if (json.contains("retweeted_status")) {
        QJsonObject retweetObject = json["retweeted_status"].toObject();

        QTweetStatus rtStatus = jsonObjectToStatus(retweetObject);

        status.setRetweetedStatus(rtStatus);
    }

    //parse place id if it's not null
    QJsonValue placeValue = json["place"];
    if (!placeValue.isNull()) {
        QTweetPlace place = jsonObjectToPlace(placeValue.toObject());
        status.setPlace(place);
    }

    //check if contains entities
    if (json.contains("entities")) {
        QJsonObject entitiesObject = json["entities"].toObject();

        //url entities
        QJsonArray urlEntitiesList = entitiesObject["urls"].toArray();

        for (int i = 0; i < urlEntitiesList.size(); ++i) {
            QTweetEntityUrl urlEntity = jsonObjectToEntityUrl(urlEntitiesList[i].toObject());

            status.addUrlEntity(urlEntity);
        }

        //hashtag entities
        QJsonArray hashtagEntitiesList = entitiesObject["hashtags"].toArray();

        for (int i = 0; i < hashtagEntitiesList.size(); ++i) {
            QTweetEntityHashtag hashtagEntity = jsonObjectToEntityHashtag(hashtagEntitiesList[i].toObject());

            status.addHashtagEntity(hashtagEntity);
        }

        //user mentions
        QJsonArray userMentionsEntitiesList = entitiesObject["user_mentions"].toArray();

        for (int i = 0; i < userMentionsEntitiesList.count(); ++i) {
            QTweetEntityUserMentions userMentionsEntity = jsonObjectToEntityUserMentions(userMentionsEntitiesList[i].toObject());

            status.addUserMentionsEntity(userMentionsEntity);
        }

        //media
        QJsonArray mediaEntitiesList = entitiesObject["media"].toArray();

        for (int i = 0; i < mediaEntitiesList.count(); ++i) {
            QTweetEntityMedia mediaEntity = jsonObjectToEntityMedia(mediaEntitiesList[i].toObject());

            status.addMediaEntity(mediaEntity);
        }
    }

    return status;
}

QTweetUser QTweetConvert::jsonObjectToUser(const QJsonObject &jsonObject)
{
    QTweetUser userInfo;

    userInfo.setId(static_cast<qint64>(jsonObject.value("id_str").toString().toLong()));

    if (jsonObject.contains("name")) {
        userInfo.setName(jsonObject.value("name").toString());
        userInfo.setLocation(jsonObject.value("location").toString());
        userInfo.setprofileImageUrl(jsonObject.value("profile_image_url").toString());
        userInfo.setCreatedAt(jsonObject.value("created_at").toString());
        userInfo.setFavouritesCount(static_cast<int>(jsonObject.value("favourites_count").toDouble()));
        userInfo.setUrl(jsonObject.value("url").toString());
        userInfo.setUtcOffset(static_cast<int>(jsonObject.value("utc_offset").toDouble()));
        userInfo.setProtected(jsonObject.value("protected").toBool());
        userInfo.setFollowersCount(static_cast<int>(jsonObject.value("followers_count").toDouble()));
        userInfo.setVerified(jsonObject.value("verified").toBool());
        userInfo.setGeoEnabled(jsonObject.value("geo_enabled").toBool());
        userInfo.setDescription(jsonObject.value("description").toString());
        userInfo.setTimezone(jsonObject.value("time_zone").toString());
        userInfo.setStatusesCount(static_cast<int>(jsonObject.value("statuses_count").toDouble()));
        userInfo.setScreenName(jsonObject.value("screen_name").toString());
        userInfo.setContributorsEnabled(jsonObject.value("contributors_enabled").toBool());
        userInfo.setListedCount(static_cast<int>(jsonObject.value("listed_count").toDouble()));
        userInfo.setLang(jsonObject.value("lang").toString());
	userInfo.setFollowing(jsonObject.value("following").toBool());

        if (jsonObject.contains("status")) {
            QJsonObject jsonStatusObject = jsonObject.value("status").toObject();

            QTweetStatus status = jsonObjectToStatus(jsonStatusObject);
            userInfo.setStatus(status);
        }
    }

    return userInfo;
}

QList<QTweetDMStatus> QTweetConvert::jsonArrayToDirectMessagesList(const QJsonArray &jsonArray)
{
    QList<QTweetDMStatus> directMessages;

    for (int i = 0; i < jsonArray.size(); ++i) {
        QJsonObject dmObject = jsonArray[i].toObject();
        QTweetDMStatus dmStatus = jsonObjectToDirectMessage(dmObject);
        directMessages.append(dmStatus);
    }

    return directMessages;
}

QTweetDMStatus QTweetConvert::jsonObjectToDirectMessage(const QJsonObject &jsonObject)
{
    QTweetDMStatus directMessage;

    directMessage.setCreatedAt(jsonObject.value("created_at").toString());
    directMessage.setSenderScreenName(jsonObject.value("sender_screen_name").toString());

    QJsonObject jsonObjectUser = jsonObject.value("sender").toObject();
    QTweetUser sender = jsonObjectToUser(jsonObjectUser);
    directMessage.setSender(sender);

    directMessage.setText(jsonObject.value("text").toString());
    directMessage.setRecipientScreenName(jsonObject["recipient_screen_name"].toString());
    directMessage.setId(static_cast<qint64>(jsonObject["id_str"].toString().toLong()));

    QJsonObject jsonObjectRecipient = jsonObject["recipient"].toObject();
    QTweetUser recipient = jsonObjectToUser(jsonObjectRecipient);
    directMessage.setRecipient(recipient);

    directMessage.setRecipientId(static_cast<qint64>(jsonObject["recipient_id_str"].toString().toLong()));
    directMessage.setSenderId(static_cast<qint64>(jsonObject["sender_id_str"].toString().toLong()));

    return directMessage;
}

QTweetList QTweetConvert::jsonObjectToTweetList(const QJsonObject& jsonObject)
{
    QTweetList list;

    list.setMode(jsonObject["mode"].toString());
    list.setDescription(jsonObject["description"].toString());
    list.setFollowing(jsonObject["following"].toBool());
    list.setMemberCount(static_cast<int>(jsonObject["member_count"].toDouble()));
    list.setFullName(jsonObject["full_name"].toString());
    list.setSubscriberCount(static_cast<int>(jsonObject["subscriber_count"].toDouble()));
    list.setSlug(jsonObject["slug"].toString());
    list.setName(jsonObject["name"].toString());
    list.setId(static_cast<qint64>(jsonObject["id_str"].toString().toLong()));
    list.setUri(jsonObject["uri"].toString());

    if (jsonObject.contains("user")) {
        QJsonObject userMap = jsonObject["user"].toObject();

        QTweetUser user = jsonObjectToUser(userMap);

        list.setUser(user);
    }

    return list;
}

QList<QTweetUser> QTweetConvert::jsonArrayToUserInfoList(const QJsonArray& jsonArray)
{
    QList<QTweetUser> users;

    for (int i = 0; i < jsonArray.size(); ++i) {
        QTweetUser userInfo = jsonObjectToUser(jsonArray[i].toObject());

        users.append(userInfo);
    }

    return users;
}

QList<QTweetList> QTweetConvert::jsonArrayToTweetLists(const QJsonArray& jsonArray)
{
    QList<QTweetList> lists;

    for (int i = 0; i < jsonArray.size(); ++i) {
        QTweetList tweetlist = jsonObjectToTweetList(jsonArray[i].toObject());

        lists.append(tweetlist);
    }

    return lists;
}

/**
 *  Converts search result
 */
QTweetSearchResult QTweetConvert::jsonObjectToSearchResult(const QJsonObject& jsonObject)
{
    QTweetSearchResult result;

    result.setCreatedAt(jsonObject["created_at"].toString());
    result.setFromUser(jsonObject["from_user"].toString());
    result.setId(static_cast<qint64>(jsonObject["id_str"].toString().toLong()));
    result.setLang(jsonObject["iso_language_code"].toString());
    result.setProfileImageUrl(jsonObject["profile_image_url"].toString());
    result.setSource(jsonObject["source"].toString());
    result.setText(jsonObject["text"].toString());
    result.setToUser(jsonObject["to_user"].toString());

    return result;
}

QTweetSearchPageResults QTweetConvert::jsonObjectToSearchPageResults(const QJsonObject& jsonObject)
{
    QTweetSearchPageResults page;

    page.setMaxId(static_cast<qint64>(jsonObject["max_id"].toDouble()));
    page.setNextPage(jsonObject["next_page"].toString().toAscii());
    page.setPage(static_cast<int>(jsonObject["page"].toDouble()));
    page.setQuery(jsonObject["query"].toString().toAscii());
    page.setRefreshUrl(jsonObject["refresh_url"].toString().toAscii());
    page.setResultsPerPage(static_cast<int>(jsonObject["results_per_page"].toDouble()));
    page.setSinceId(static_cast<qint64>(jsonObject["since_id"].toDouble()));
    page.setTotal(static_cast<int>(jsonObject["total"].toDouble()));

    QList<QTweetSearchResult> resultList;
    QJsonArray resultArray = jsonObject["results"].toArray();

    for (int i = 0; i < resultArray.size(); ++i) {
        QTweetSearchResult result = jsonObjectToSearchResult(resultArray[i].toObject());
        resultList.append(result);
    }

    page.setResults(resultList);

    return page;
}

QTweetPlace QTweetConvert::jsonObjectToPlace(const QJsonObject& jsonObject)
{
    QTweetPlace place;

    place.setName(jsonObject["name"].toString());
    place.setCountryCode(jsonObject["country_code"].toString());
    place.setCountry(jsonObject["country"].toString());
    place.setID(jsonObject["id"].toString());
    place.setFullName(jsonObject["full_name"].toString());

    QString placeType = jsonObject["place_type"].toString();

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

    QJsonValue bbJsonValue = jsonObject["bounding_box"];

    if (!bbJsonValue.isNull()) {
        QJsonObject bbJsonObject = bbJsonValue.toObject();

        if (bbJsonObject["type"].toString() == "Polygon") {
            QJsonArray coordList = bbJsonObject["coordinates"].toArray();

            if (coordList.count() == 1) {
                QJsonArray latLongList = coordList[0].toArray();

                if (latLongList.count() == 4) {
                    QTweetGeoBoundingBox box;

                    QJsonArray coordsBottomLeft = latLongList[0].toArray();
                    box.setBottomLeft(QTweetGeoCoord(coordsBottomLeft[1].toDouble(), coordsBottomLeft[0].toDouble()));

                    QJsonArray coordsBottomRight = latLongList[1].toArray();
                    box.setBottomRight(QTweetGeoCoord(coordsBottomRight[1].toDouble(), coordsBottomRight[0].toDouble()));

                    QJsonArray coordsTopRight = latLongList[2].toArray();
                    box.setTopRight(QTweetGeoCoord(coordsTopRight[1].toDouble(), coordsTopRight[0].toDouble()));

                    QJsonArray coordsTopLeft = latLongList[3].toArray();
                    box.setTopLeft(QTweetGeoCoord(coordsTopLeft[1].toDouble(), coordsTopLeft[0].toDouble()));

                    place.setBoundingBox(box);
                }
            }
        }
    }
    return place;
}

//not to be used in timelines api, but in geo api, where place contains other places
//is it recursive responsive?
QTweetPlace QTweetConvert::jsonObjectToPlaceRecursive(const QJsonObject& jsonObject)
{
    QTweetPlace place;

    place.setName(jsonObject["name"].toString());
    place.setCountryCode(jsonObject["country_code"].toString());
    place.setCountry(jsonObject["country"].toString());
    place.setID(jsonObject["id"].toString());
    place.setFullName(jsonObject["full_name"].toString());

    QString placeType = jsonObject["place_type"].toString();

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

    QJsonValue bbVar = jsonObject["bounding_box"];

    if (!bbVar.isNull()) {
        QJsonObject bbObject = bbVar.toObject();

        if (bbObject["type"].toString() == "Polygon") {
            QJsonArray coordList = bbObject["coordinates"].toArray();

            if (coordList.count() == 1) {
                QJsonArray latLongList = coordList[0].toArray();

                if (latLongList.count() == 4) {
                    QTweetGeoBoundingBox box;

                    QJsonArray coordsBottomLeft = latLongList[0].toArray();
                    box.setBottomLeft(QTweetGeoCoord(coordsBottomLeft[1].toDouble(), coordsBottomLeft[0].toDouble()));

                    QJsonArray coordsBottomRight = latLongList[1].toArray();
                    box.setBottomRight(QTweetGeoCoord(coordsBottomRight[1].toDouble(), coordsBottomRight[0].toDouble()));

                    QJsonArray coordsTopRight = latLongList[2].toArray();
                    box.setTopRight(QTweetGeoCoord(coordsTopRight[1].toDouble(), coordsTopRight[0].toDouble()));

                    QJsonArray coordsTopLeft = latLongList[3].toArray();
                    box.setTopLeft(QTweetGeoCoord(coordsTopLeft[1].toDouble(), coordsTopLeft[0].toDouble()));

                    place.setBoundingBox(box);
                }
            }
        }
    }

    QJsonArray containedArray = jsonObject["contained_within"].toArray();

    QList<QTweetPlace> containedInPlacesList;

    if (!containedArray.isEmpty()) {
        for (int i = 0; i < containedArray.size(); ++i) {
            QTweetPlace containedPlace = jsonObjectToPlaceRecursive(containedArray[i].toObject());

            containedInPlacesList.append(containedPlace);
        }
    }

    place.setContainedWithin(containedInPlacesList);

    return place;
}

QList<QTweetPlace> QTweetConvert::jsonObjectToPlaceList(const QJsonObject& jsonObject)
{
    QList<QTweetPlace> placeList;

    QJsonObject resultObject = jsonObject["result"].toObject();
    QJsonArray placesArray = resultObject["places"].toArray();

    for (int i = 0; i < placesArray.size(); ++i) {
        QTweetPlace place = jsonObjectToPlaceRecursive(placesArray[i].toObject());
        placeList.append(place);
    }

    return placeList;
}

QTweetEntityUrl QTweetConvert::jsonObjectToEntityUrl(const QJsonObject &jsonObject)
{
    QString url = jsonObject["url"].toString();
    QString displayUrl = jsonObject["display_url"].toString();
    QString expandedUrl = jsonObject["expanded_url"].toString();

    QJsonArray indices = jsonObject["indices"].toArray();

    QTweetEntityUrl urlEntity;
    urlEntity.setUrl(url);
    urlEntity.setDisplayUrl(displayUrl);
    urlEntity.setExpandedUrl(expandedUrl);
    urlEntity.setLowerIndex((int)indices[0].toDouble());
    urlEntity.setHigherIndex((int)indices[1].toDouble());

    return urlEntity;
}

QTweetEntityHashtag QTweetConvert::jsonObjectToEntityHashtag(const QJsonObject &jsonObject)
{
    QTweetEntityHashtag hashtagEntity;

    hashtagEntity.setText(jsonObject["text"].toString());

    QJsonArray indices = jsonObject["indices"].toArray();
    hashtagEntity.setLowerIndex((int)indices[0].toDouble());
    hashtagEntity.setHigherIndex((int)indices[1].toDouble());

    return hashtagEntity;
}

QTweetEntityUserMentions QTweetConvert::jsonObjectToEntityUserMentions(const QJsonObject &jsonObject)
{
    QTweetEntityUserMentions userMentionsEntity;

    userMentionsEntity.setScreenName(jsonObject["screen_name"].toString());
    userMentionsEntity.setName(jsonObject["name"].toString());
    userMentionsEntity.setUserid(static_cast<qint64>(jsonObject["id"].toDouble()));

    QJsonArray indicesObject = jsonObject["indices"].toArray();
    userMentionsEntity.setLowerIndex((int)indicesObject[0].toDouble());
    userMentionsEntity.setHigherIndex((int)indicesObject[1].toDouble());

    return userMentionsEntity;
}

QTweetEntityMedia QTweetConvert::jsonObjectToEntityMedia(const QJsonObject &jsonObject)
{
    QTweetEntityMedia entityMedia;

    entityMedia.setID(jsonObject["id_str"].toString());
    entityMedia.setMediaUrl(jsonObject["media_url"].toString());
    entityMedia.setMediaUrlHttps(jsonObject["media_url_https"].toString());
    entityMedia.setUrl(jsonObject["url"].toString());
    entityMedia.setDisplayUrl(jsonObject["display_url"].toString());
    entityMedia.setExpandedUrl(jsonObject["expanded_url"].toString());

    QJsonObject sizesObject = jsonObject["sizes"].toObject();

    QJsonObject largeObject = sizesObject["large"].toObject();
    QSize large;
    large.setWidth(static_cast<int>(largeObject["w"].toDouble()));
    large.setHeight(static_cast<int>(largeObject["h"].toDouble()));

    entityMedia.setSize(large, QTweetEntityMedia::LARGE);

    QJsonObject mediumObject = sizesObject["medium"].toObject();
    QSize medium;
    medium.setWidth(static_cast<int>(mediumObject["w"].toDouble()));
    medium.setHeight(static_cast<int>(mediumObject["h"].toDouble()));

    entityMedia.setSize(medium, QTweetEntityMedia::MEDIUM);

    QJsonObject smallObject = sizesObject["small"].toObject();
    QSize small;
    small.setWidth(static_cast<int>(smallObject["w"].toDouble()));
    small.setHeight(static_cast<int>(smallObject["h"].toDouble()));

    entityMedia.setSize(small, QTweetEntityMedia::SMALL);

    QJsonObject thumbObject = sizesObject["thumb"].toObject();
    QSize thumb;
    thumb.setWidth(static_cast<int>(thumbObject["w"].toDouble()));
    thumb.setHeight(static_cast<int>(thumbObject["h"].toDouble()));

    entityMedia.setSize(thumb, QTweetEntityMedia::THUMB);

    QJsonArray indicesObject = jsonObject["indices"].toArray();
    entityMedia.setLowerIndex(static_cast<int>(indicesObject[0].toDouble()));
    entityMedia.setHigherIndex(static_cast<int>(indicesObject[1].toDouble()));

    return entityMedia;
}
