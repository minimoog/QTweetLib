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

QList<QTweetStatus> QTweetConvert::cJSONToStatusList(cJSON *root)
{
    QList<QTweetStatus> statuses;

    if (root->type == cJSON_Array) {
        int size = cJSON_GetArraySize(root);

        for (int i = 0; i < size; ++i) {
            cJSON *statusObject = cJSON_GetArrayItem(root, i);

            QTweetStatus tweetStatus = cJSONToStatus(statusObject);
            statuses.append(tweetStatus);
        }
    }

    return statuses;
}

QTweetStatus QTweetConvert::cJSONToStatus(cJSON *root)
{
    QTweetStatus status;

    if (root->type == cJSON_Object) {
        status.setCreatedAt(QString::fromUtf8(cJSON_GetObjectItem(root, "created_at")->valuestring));
        status.setText(QString::fromUtf8(cJSON_GetObjectItem(root, "text")->valuestring));
        status.setId((qint64)cJSON_GetObjectItem(root, "id")->valuedouble);
        status.setInReplyToUserId((qint64)cJSON_GetObjectItem(root, "in_reply_to_user_id")->valuedouble);
        status.setInReplyToScreenName(QString::fromUtf8(cJSON_GetObjectItem(root, "in_reply_to_screen_name")->valuestring));
        status.setFavorited(cJSON_GetObjectItem(root, "favorited")->valuestring);   //### TODO

        cJSON *userObject = cJSON_GetObjectItem(root, "user");
        if (userObject) {
            QTweetUser user = cJSONToUser(userObject);
            status.setUser(user);
        }

        status.setSource(QString::fromUtf8(cJSON_GetObjectItem(root, "source")->valuestring));
        status.setInReplyToStatusId((qint64)cJSON_GetObjectItem(root, "in_reply_to_status_id")->valuedouble);

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

    user.setId((qint64)cJSON_GetObjectItem(root, "id")->valuedouble);

    cJSON *nameObject = cJSON_GetObjectItem(root, "name");

    if (nameObject) { // ### TODO FIX
        user.setName(QString::fromUtf8(cJSON_GetObjectItem(root, "name")->valuestring));
        user.setLocation(QString::fromUtf8(cJSON_GetObjectItem(root, "location")->valuestring));
        user.setprofileImageUrl(QString::fromUtf8(cJSON_GetObjectItem(root, "profile_image_url")->valuestring));
        user.setCreatedAt(QString::fromUtf8(cJSON_GetObjectItem(root, "created_at")->valuestring));
        user.setFavouritesCount(cJSON_GetObjectItem(root, "favourites_count")->valueint);
        user.setUrl(QString::fromUtf8(cJSON_GetObjectItem(root, "url")->valuestring));
        user.setUtcOffset(cJSON_GetObjectItem(root, "utc_offset")->valueint);
        //user.setProtected(QString::fromUtf8(cJSON_GetObjectItem(root, "protected")->valuestring)); // ### TODO
        user.setFollowersCount(cJSON_GetObjectItem(root, "followers_count")->valueint);
        //user.setVerified(QString::fromUtf8(cJSON_GetObjectItem(root, "verified")->valuestring)); // ### TODO
        //user.setGeoEnabled(QString::fromUtf8(cJSON_GetObjectItem(root, "geo_enabled")->valuestring)); // ### TODO
        user.setDescription(QString::fromUtf8(cJSON_GetObjectItem(root, "description")->valuestring));
        user.setTimezone(QString::fromUtf8(cJSON_GetObjectItem(root, "time_zone")->valuestring));
        user.setFriendsCount(cJSON_GetObjectItem(root, "friends_count")->valueint);
        user.setStatusesCount(cJSON_GetObjectItem(root, "statuses_count")->valueint);
        user.setScreenName(QString::fromUtf8(cJSON_GetObjectItem(root, "screen_name")->valuestring));
        //user.setContributorsEnabled(QString::fromUtf8(cJSON_GetObjectItem(root, "contributors_enabled")->valuestring)); // ### TODO
        user.setListedCount(cJSON_GetObjectItem(root, "listed_count")->valueint);
        user.setLang(QString::fromUtf8(cJSON_GetObjectItem(root, "lang")->valuestring));

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
            // ### TODO: cJSONtoDirectMessage
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
        directMessage.setCreatedAt(QString::fromUtf8(cJSON_GetObjectItem(root, "created_at")->valuestring));
        directMessage.setSenderScreenName(QString::fromUtf8(cJSON_GetObjectItem(root, "sender_screen_name")->valuestring));

        cJSON *senderObject = cJSON_GetObjectItem(root, "sender");
        QTweetUser sender = cJSONToUser(senderObject);

        directMessage.setSender(sender);

        directMessage.setText(QString::fromUtf8(cJSON_GetObjectItem(root, "text")->valuestring));
        directMessage.setRecipientScreenName(QString::fromUtf8(cJSON_GetObjectItem(root, "recipient_screen_name")->valuestring));
        directMessage.setId((qint64)cJSON_GetObjectItem(root, "id")->valuedouble);

        cJSON *recipientObject = cJSON_GetObjectItem(root, "recipient");
        QTweetUser recipient = cJSONToUser(recipientObject);

        directMessage.setRecipient(recipient);

        directMessage.setRecipientId((qint64)cJSON_GetObjectItem(root, "recipient_id")->valuedouble);
        directMessage.setSenderId((qint64)cJSON_GetObjectItem(root, "sender_id")->valuedouble);
    }

    return directMessage;
}

QTweetList QTweetConvert::cJSONToTweetList(cJSON *root)
{
    QTweetList list;

    if (root->type == cJSON_Object) {
        list.setMode(QString::fromUtf8(cJSON_GetObjectItem(root, "mode")->valuestring));
        list.setDescription(QString::fromUtf8(cJSON_GetObjectItem(root, "description")->valuestring));
        list.setFollowing(cJSON_GetObjectItem(root, "following")->valueint);
        list.setMemberCount(cJSON_GetObjectItem(root, "member_count")->valueint);
        list.setFullName(QString::fromUtf8(cJSON_GetObjectItem(root, "full_name")->valuestring));
        list.setSubscriberCount(cJSON_GetObjectItem(root, "subscriber_count")->valueint);
        list.setSlug(QString::fromUtf8(cJSON_GetObjectItem(root, "slug")->valuestring));
        list.setName(QString::fromUtf8(cJSON_GetObjectItem(root, "name")->valuestring));
        list.setId((qint64)cJSON_GetObjectItem(root, "id")->valuedouble);
        list.setUri(QString::fromUtf8(cJSON_GetObjectItem(root, "uri")->valuestring));

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
        result.setCreatedAt(QString::fromUtf8(cJSON_GetObjectItem(root, "created_at")->valuestring));
        result.setFromUser(QString::fromUtf8(cJSON_GetObjectItem(root, "from_user")->valuestring));
        result.setId((qint64)cJSON_GetObjectItem(root, "id")->valuedouble);
        result.setLang(QString::fromUtf8(cJSON_GetObjectItem(root, "iso_language_code")->valuestring));
        result.setProfileImageUrl(QString::fromUtf8(cJSON_GetObjectItem(root, "profile_image_url")->valuestring));
        result.setSource(QString::fromUtf8(cJSON_GetObjectItem(root, "source")->valuestring));
        result.setText(QString::fromUtf8(cJSON_GetObjectItem(root, "text")->valuestring));
        result.setToUser(QString::fromUtf8(cJSON_GetObjectItem(root, "to_user")->valuestring));
    }

    return result;
}

QTweetSearchPageResults QTweetConvert::cJSONToSearchPageResults(cJSON *root)
{
    QTweetSearchPageResults page;

    if (root->type == cJSON_Object) {
        page.setMaxId((qint64)cJSON_GetObjectItem(root, "max_id")->valuedouble);
        page.setNextPage(cJSON_GetObjectItem(root, "next_page")->valuestring);  //?
        page.setPage(cJSON_GetObjectItem(root, "page")->valueint);
        page.setQuery(cJSON_GetObjectItem(root, "query")->valuestring);  //?
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

QTweetPlace QTweetConvert::cJSONToPlace(cJSON *root)
{
    QTweetPlace place;

    if (root->type == cJSON_Object) {
        place.setName(QString::fromUtf8(cJSON_GetObjectItem(root, "name")->valuestring));
        place.setCountryCode(QString::fromUtf8(cJSON_GetObjectItem(root, "country_code")->valuestring));
        place.setCountry(QString::fromUtf8(cJSON_GetObjectItem(root, "country")->valuestring));
        place.setID(QString::fromUtf8(cJSON_GetObjectItem(root, "id")->valuestring));
        place.setFullName(QString::fromUtf8(cJSON_GetObjectItem(root, "full_name")->valuestring));

        QString placeType = QString::fromUtf8(cJSON_GetObjectItem(root, "place_type")->valuestring);

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
            QString type = QString::fromUtf8(cJSON_GetObjectItem(bboxObject, "type")->valuestring);

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
        place.setName(QString::fromUtf8(cJSON_GetObjectItem(root, "name")->valuestring));
        place.setCountryCode(QString::fromUtf8(cJSON_GetObjectItem(root, "country_code")->valuestring));
        place.setCountry(QString::fromUtf8(cJSON_GetObjectItem(root, "country")->valuestring));
        place.setID(QString::fromUtf8(cJSON_GetObjectItem(root, "id")->valuestring));
        place.setFullName(QString::fromUtf8(cJSON_GetObjectItem(root, "full_name")->valuestring));

        QString placeType = QString::fromUtf8(cJSON_GetObjectItem(root, "place_type")->valuestring);

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
            QString type = QString::fromUtf8(cJSON_GetObjectItem(bboxObject, "type")->valuestring);

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

    urlEntity.setUrl(QString::fromUtf8(cJSON_GetObjectItem(root, "url")->valuestring));
    urlEntity.setDisplayUrl(QString::fromUtf8(cJSON_GetObjectItem(root, "display_url")->valuestring));
    urlEntity.setExpandedUrl(QString::fromUtf8(cJSON_GetObjectItem(root, "expanded_url")->valuestring));

    return urlEntity;
}

QTweetEntityHashtag QTweetConvert::cJSONToEntityHashtag(cJSON *root)
{
    QTweetEntityHashtag hashtagEntity;

    hashtagEntity.setText(QString::fromUtf8(cJSON_GetObjectItem(root, "text")->valuestring));

    return hashtagEntity;
}

QTweetEntityUserMentions QTweetConvert::cJSONToEntityUserMentions(cJSON *root)
{
    QTweetEntityUserMentions userMentionsEntity;

    userMentionsEntity.setScreenName(QString::fromUtf8(cJSON_GetObjectItem(root, "screen_name")->valuestring));
    userMentionsEntity.setName(QString::fromUtf8(cJSON_GetObjectItem(root, "name")->valuestring));
    userMentionsEntity.setUserid((qint64)cJSON_GetObjectItem(root, "id")->valuedouble);

    return userMentionsEntity;
}
