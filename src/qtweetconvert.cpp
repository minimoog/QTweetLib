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

static QString cJSONGetString(cJSON *object, const char * value)
{
    cJSON *item = cJSON_GetObjectItem(object, value);
    Q_ASSERT(item);

    return QString::fromUtf8(item->valuestring);
}

static qint64 cJSONGetID(cJSON *object, const char * value)
{
    cJSON *item = cJSON_GetObjectItem(object, value);
    Q_ASSERT(item);

    QString str_id(item->valuestring);

    return str_id.toLongLong();
}

static bool cJSONGetBoolean(cJSON *object, const char * value)
{
    cJSON *item = cJSON_GetObjectItem(object, value);
    Q_ASSERT(item);

    if (item->type == cJSON_True)
        return true;
    else
        return false;
}

static int cJSONGetInt(cJSON *object, const char * value)
{
    cJSON *item = cJSON_GetObjectItem(object, value);
    Q_ASSERT(item);

    return item->valueint;
}

static QByteArray cJSONGetByteArray(cJSON *object, const char *value)
{
    cJSON *item = cJSON_GetObjectItem(object, value);
    Q_ASSERT(item);

    return QByteArray(item->valuestring);
}

QList<QTweetStatus> QTweetConvert::cJSONToStatusList(cJSON *root)
{
    QList<QTweetStatus> statuses;

    if (root->type == cJSON_Array) {
        int size = cJSON_GetArraySize(root);

        for (int i = 0; i < size; ++i) {
            cJSON *statusObject = cJSON_GetArrayItem(root, i);
            Q_ASSERT(statusObject);

            QTweetStatus tweetStatus = cJSONToStatus(statusObject);
            statuses.append(tweetStatus);
        }
    }

    return statuses;
}

QTweetStatus QTweetConvert::cJSONToStatus(cJSON *root)
{
    Q_ASSERT(root);
    QTweetStatus status;

    if (root->type == cJSON_Object) {
        status.setCreatedAt(cJSONGetString(root, "created_at"));
        status.setText(cJSONGetString(root, "text"));
        status.setId(cJSONGetID(root, "id_str"));
        status.setInReplyToUserId(cJSONGetID(root, "in_reply_to_user_id_str"));
        status.setInReplyToScreenName(cJSONGetString(root, "in_reply_to_screen_name"));
        status.setFavorited(cJSONGetBoolean(root, "favorited"));

        cJSON *userObject = cJSON_GetObjectItem(root, "user");
        if (userObject) {
            QTweetUser user = cJSONToUser(userObject);
            status.setUser(user);
        }

        status.setSource(cJSONGetString(root, "source"));
        status.setInReplyToStatusId(cJSONGetID(root, "in_reply_to_status_id_str"));

        cJSON *rtObject = cJSON_GetObjectItem(root, "retweeted_status");
        if (rtObject) {
            QTweetStatus rtStatus = cJSONToStatus(rtObject);
            status.setRetweetedStatus(rtStatus);
        }

        cJSON *placeObject = cJSON_GetObjectItem(root, "place");

        if (placeObject->type != cJSON_NULL) {
            QTweetPlace place = cJSONToPlace(placeObject);

            status.setPlace(place);
        }

        cJSON *entObject = cJSON_GetObjectItem(root, "entities");
        if (entObject) {
            cJSON *urlArray = cJSON_GetObjectItem(entObject, "urls");

            for (int i = 0; i < cJSON_GetArraySize(urlArray); ++i) {
                cJSON *urlObject = cJSON_GetArrayItem(urlArray, i);
                QTweetEntityUrl urlEntity = cJSONToEntityUrl(urlObject);

                status.addUrlEntity(urlEntity);
            }

            cJSON *hashtagsArray = cJSON_GetObjectItem(entObject, "hashtags");

            for (int i = 0; i < cJSON_GetArraySize(hashtagsArray); ++i) {
                cJSON *hashtagObject = cJSON_GetArrayItem(hashtagsArray, i);
                QTweetEntityHashtag hashtagEntity = cJSONToEntityHashtag(hashtagObject);

                status.addHashtagEntity(hashtagEntity);
            }

            cJSON *userMentionsArray = cJSON_GetObjectItem(entObject, "user_mentions");

            for (int i = 0; i < cJSON_GetArraySize(userMentionsArray); ++i) {
                cJSON *userMentionObject =cJSON_GetArrayItem(userMentionsArray, i);
                QTweetEntityUserMentions userMentionsEntity = cJSONToEntityUserMentions(userMentionObject);

                status.addUserMentionsEntity(userMentionsEntity);
            }
        }
    }

    return status;
}

QTweetUser QTweetConvert::cJSONToUser(cJSON *root)
{
    QTweetUser user;

    user.setId(cJSONGetID(root,"id_str"));

    cJSON *nameObject = cJSON_GetObjectItem(root, "name");

    if (nameObject) {
        user.setName(cJSONGetString(root, "name"));
        user.setLocation(cJSONGetString(root, "location"));
        user.setprofileImageUrl(cJSONGetString(root, "profile_image_url"));
        user.setCreatedAt(cJSONGetString(root, "created_at"));
        user.setFavouritesCount(cJSONGetInt(root, "favourites_count"));
        user.setUrl(cJSONGetString(root, "url"));
        user.setUtcOffset(cJSONGetInt(root, "utc_offset"));
        user.setProtected(cJSONGetBoolean(root, "protected"));
        user.setFollowersCount(cJSONGetInt(root, "followers_count"));
        user.setVerified(cJSONGetBoolean(root, "verified"));
        user.setGeoEnabled(cJSONGetBoolean(root, "geo_enabled"));
        user.setDescription(cJSONGetString(root, "description"));
        user.setTimezone(cJSONGetString(root, "time_zone"));
        user.setFriendsCount(cJSONGetInt(root, "friends_count"));
        user.setStatusesCount(cJSONGetInt(root, "statuses_count"));
        user.setScreenName(cJSONGetString(root, "screen_name"));
        user.setContributorsEnabled(cJSONGetBoolean(root, "contributors_enabled"));
        user.setListedCount(cJSONGetInt(root, "listed_count"));
        user.setLang(cJSONGetString(root, "lang"));

        cJSON *statusObject = cJSON_GetObjectItem(root, "status");

        if (statusObject) {
            QTweetStatus status = cJSONToStatus(statusObject);

            user.setStatus(status);
        }
    }
    return user;
}

QList<QTweetDMStatus> QTweetConvert::cJSONToDirectMessagesList(cJSON *root)
{
    QList<QTweetDMStatus> directMessages;

    if (root->type == cJSON_Array) {
        int size = cJSON_GetArraySize(root);

        for (int i = 0; i < size; i++) {
            cJSON *dmObject = cJSON_GetArrayItem(root, i);
            QTweetDMStatus dmStatus = cJSONToDirectMessage(dmObject);
            directMessages.append(dmStatus);
        }
    }

    return directMessages;
}

QTweetDMStatus QTweetConvert::cJSONToDirectMessage(cJSON *root)
{
    QTweetDMStatus directMessage;

    if (root->type == cJSON_Object) {
        directMessage.setCreatedAt(cJSONGetString(root, "created_at"));
        directMessage.setSenderScreenName(cJSONGetString(root, "sender_screen_name"));

        cJSON *senderObject = cJSON_GetObjectItem(root, "sender");
        QTweetUser sender = cJSONToUser(senderObject);

        directMessage.setSender(sender);

        directMessage.setText(cJSONGetString(root, "text"));
        directMessage.setRecipientScreenName(cJSONGetString(root, "recipient_screen_name"));
        directMessage.setId(cJSONGetID(root, "id_str"));

        cJSON *recipientObject = cJSON_GetObjectItem(root, "recipient");
        QTweetUser recipient = cJSONToUser(recipientObject);

        directMessage.setRecipient(recipient);

        directMessage.setRecipientId(cJSONGetID(root, "recipient_id_str"));
        directMessage.setSenderId(cJSONGetID(root, "sender_id_str"));
    }

    return directMessage;
}

QTweetList QTweetConvert::cJSONToTweetList(cJSON *root)
{
    QTweetList list;

    if (root->type == cJSON_Object) {
        list.setMode(cJSONGetString(root, "mode"));
        list.setDescription(cJSONGetString(root, "description"));
        list.setFollowing(cJSONGetInt(root, "following"));
        list.setMemberCount(cJSONGetInt(root, "member_count"));
        list.setFullName(cJSONGetString(root, "full_name"));
        list.setSubscriberCount(cJSONGetInt(root, "subscriber_count"));
        list.setSlug(cJSONGetString(root, "slug"));
        list.setName(cJSONGetString(root, "name"));
        list.setId(cJSONGetID(root, "id_str"));
        list.setUri(cJSONGetString(root, "uri"));

        cJSON *userObject = cJSON_GetObjectItem(root, "user");

        if (userObject) {
            QTweetUser user = cJSONToUser(userObject);

            list.setUser(user);
        }
    }

    return list;
}

QList<QTweetUser> QTweetConvert::cJSONToUserInfoList(cJSON *root)
{
    QList<QTweetUser> users;

    if (root->type == cJSON_Array) {
        int size = cJSON_GetArraySize(root);

        for (int i = 0; i < size; i++) {
            cJSON *userObject = cJSON_GetArrayItem(root, i);

            QTweetUser user = cJSONToUser(userObject);

            users.append(user);
        }
    }

    return users;
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

QTweetSearchResult QTweetConvert::cJSONToSearchResult(cJSON *root)
{
    QTweetSearchResult result;

    if (root->type == cJSON_Object) {
        result.setCreatedAt(cJSONGetString(root, "created_at"));
        result.setFromUser(cJSONGetString(root, "from_user"));
        result.setId(cJSONGetID(root, "id_str"));
        result.setLang(cJSONGetString(root, "iso_language_code"));
        result.setProfileImageUrl(cJSONGetString(root, "profile_image_url"));
        result.setSource(cJSONGetString(root, "source"));
        result.setText(cJSONGetString(root, "text"));
        result.setToUser(cJSONGetString(root, "to_user"));
    }

    return result;
}

QTweetSearchPageResults QTweetConvert::cJSONToSearchPageResults(cJSON *root)
{
    QTweetSearchPageResults page;

    if (root->type == cJSON_Object) {
        page.setMaxId(cJSONGetID(root, "max_id_str"));
        page.setNextPage(cJSONGetByteArray(root, "next_page"));
        page.setPage(cJSONGetInt(root, "page"));
        page.setQuery(cJSONGetByteArray(root, "query"));
        page.setRefreshUrl(cJSONGetByteArray(root, "refresh_url"));
        page.setResultsPerPage(cJSONGetInt(root, "results_per_page"));
        page.setSinceId(cJSONGetID(root, "since_id_str"));

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

QTweetPlace QTweetConvert::cJSONToPlace(cJSON *root)
{
    QTweetPlace place;

    if (root->type == cJSON_Object) {
        place.setName(cJSONGetString(root, "name"));
        place.setCountryCode(cJSONGetString(root, "country_code"));
        place.setCountry(cJSONGetString(root, "country"));
        place.setID(cJSONGetString(root, "id"));
        place.setFullName(cJSONGetString(root, "full_name"));

        QString placeType = cJSONGetString(root, "place_type");

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
            QString type = cJSONGetString(bboxObject, "type");

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

QTweetPlace QTweetConvert::cJSONToPlaceRecursive(cJSON *root)
{
    QTweetPlace place;

    if (root->type == cJSON_Object) {
        place.setName(cJSONGetString(root, "name"));
        place.setCountryCode(cJSONGetString(root, "country_code"));
        place.setCountry(cJSONGetString(root, "country"));
        place.setID(cJSONGetString(root, "id"));
        place.setFullName(cJSONGetString(root, "full_name"));

        QString placeType = cJSONGetString(root, "place_type");

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
            QString type = cJSONGetString(bboxObject, "type");

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

QTweetEntityUrl QTweetConvert::cJSONToEntityUrl(cJSON *root)
{
    QTweetEntityUrl urlEntity;

    urlEntity.setUrl(cJSONGetString(root, "url"));
    urlEntity.setDisplayUrl(cJSONGetString(root, "display_url"));
    urlEntity.setExpandedUrl(cJSONGetString(root, "expanded_url"));

    return urlEntity;
}

QTweetEntityHashtag QTweetConvert::cJSONToEntityHashtag(cJSON *root)
{
    QTweetEntityHashtag hashtagEntity;

    hashtagEntity.setText(cJSONGetString(root, "text"));

    return hashtagEntity;
}

QTweetEntityUserMentions QTweetConvert::cJSONToEntityUserMentions(cJSON *root)
{
    QTweetEntityUserMentions userMentionsEntity;

    userMentionsEntity.setScreenName(cJSONGetString(root, "screen_name"));
    userMentionsEntity.setName(cJSONGetString(root, "name"));
    userMentionsEntity.setUserid(cJSONGetID(root, "id_str"));

    return userMentionsEntity;
}
