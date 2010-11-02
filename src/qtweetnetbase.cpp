/* Copyright (c) 2010, Antonie Jovanoski
 *
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact e-mail: Antonie Jovanoski <minimoog77_at_gmail.com>
 */

#include <QtDebug>
#include <QThreadPool>
#include <QNetworkReply>
#include "qtweetnetbase.h"
#include "qtweetstatus.h"
#include "qtweetdmstatus.h"
#include "qtweetuser.h"
#include "qtweetlist.h"
#include "qtweetsearchresult.h"
#include "qtweetsearchpageresults.h"
#include "qtweetplace.h"
#include "qjson/parserrunnable.h"
#include "qjson/parser.h"

/**
 *   Constructor
 */
QTweetNetBase::QTweetNetBase(QObject *parent) :
    QObject(parent), m_oauthTwitter(0), m_jsonParsingEnabled(true), m_authentication(true)
{
}

/**
 *   Constructor
 *   @param oauthTwitter OAuth Twitter
 *   @param parent QObject parent
 */
QTweetNetBase::QTweetNetBase(OAuthTwitter *oauthTwitter, QObject *parent) :
        QObject(parent), m_oauthTwitter(oauthTwitter), m_jsonParsingEnabled(true), m_authentication(true)
{

}

/**
 *  Desctructor
 */
QTweetNetBase::~QTweetNetBase()
{
}

/**
 *   Sets OAuth Twitter authorization object
 *   @param oauthTwitter OAuth Twitter object
 */
void QTweetNetBase::setOAuthTwitter(OAuthTwitter *oauthTwitter)
{
    m_oauthTwitter = oauthTwitter;
}

/**
 *   Gets OAuth Twitter authorization object
 *   @return OAuth Twitter object
 */
OAuthTwitter* QTweetNetBase::oauthTwitter() const
{
    return m_oauthTwitter;
}

/**
 *  Gets response
 */
QByteArray QTweetNetBase::response() const
{
    return m_response;
}

/**
 *  Gets last error message
 */
QString QTweetNetBase::lastErrorMessage() const
{
    return m_lastErrorMessage;
}

/**
 *  Enables/disables json parsing
 *  @remarks When disabled only finished and error signals are emited
 */
void QTweetNetBase::setJsonParsingEnabled(bool enable)
{
    m_jsonParsingEnabled = enable;
}

/**
 *  Checks if its json parsing enabled
 */
bool QTweetNetBase::isJsonParsingEnabled() const
{
    return m_jsonParsingEnabled;
}

/**
 *  Enables/disables oauth authentication
 *  @remarks Most of classes requires authentication
 */
void QTweetNetBase::setAuthenticationEnabled(bool enable)
{
    m_authentication = enable;
}

/**
 *  Checks if authentication is enabled
 */
bool QTweetNetBase::isAuthenticationEnabled() const
{
    return m_authentication;
}

/**
 *  Parses json response
 */
void QTweetNetBase::parseJson(const QByteArray &jsonData)
{
    QJson::ParserRunnable *jsonParser = new QJson::ParserRunnable;
    jsonParser->setData(jsonData);

    connect(jsonParser, SIGNAL(parsingFinished(QVariant,bool,QString)),
            this, SLOT(parsingJsonFinished(QVariant,bool,QString)));

    QThreadPool::globalInstance()->start(jsonParser);
}

/**
 *  Called after response from twitter
 */
void QTweetNetBase::reply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            m_response = reply->readAll();
            emit finished(m_response);

            if (isJsonParsingEnabled())
                parseJson(m_response);
        } else {
            m_response = reply->readAll();

            //dump error
            qDebug() << "Network error: " << reply->error();
            qDebug() << "Error string: " << reply->errorString();
            qDebug() << "Error response: " << m_response;

            //HTTP status code
            int httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

            //try to json parse the error response
            QJson::Parser parser;
            bool ok;

            QVariantMap errMsgMap = parser.parse(m_response, &ok).toMap();
            if (!ok) {
                m_lastErrorMessage.clear();
            } else {
                //QString request = errMsgMap["request"].toString();
                m_lastErrorMessage = errMsgMap["error"].toString();
            }

            switch (httpStatus) {
            case NotModified:
            case BadRequest:
            case Unauthorized:
            case Forbidden:
            case NotFound:
            case NotAcceptable:
            case EnhanceYourCalm:
            case InternalServerError:
            case BadGateway:
            case ServiceUnavailable:
                emit error(static_cast<ErrorCode>(httpStatus), m_lastErrorMessage);
                break;
            default:
                emit error(UnknownError, m_lastErrorMessage);
            }
        }
        reply->deleteLater();
    }
}

/**
 *  Sets last error message
 */
void QTweetNetBase::setLastErrorMessage(const QString &errMsg)
{
    m_lastErrorMessage = errMsg;
}

/**
 *  Converts list of statuses
 */
QList<QTweetStatus> QTweetNetBase::variantToStatusList(const QVariant &fromParser)
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

/**
 *  Converts status
 */
QTweetStatus QTweetNetBase::variantMapToStatus(const QVariantMap &var)
{
    QTweetStatus status;

    status.setCreatedAt(var["created_at"].toString());
    status.setText(var["text"].toString());
    status.setId(var["id"].toLongLong());
    status.setInReplyToUserId(var["in_reply_to_user_id"].toLongLong());
    status.setInReplyToScreenName(var["in_reply_to_screen_name"].toString());

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

    return status;
}

/**
 *  Converts list of direct messages
 */
QList<QTweetDMStatus> QTweetNetBase::variantToDirectMessagesList(const QVariant &fromParser)
{
    QList<QTweetDMStatus> directMessages;

    QList<QVariant> listMessages = fromParser.toList();

    foreach (const QVariant& message, listMessages) {
        QTweetDMStatus dmStatus = variantMapToDirectMessage(message.toMap());
        directMessages.append(dmStatus);
    }

    return directMessages;
}

/**
 *  Converts direct message
 */
QTweetDMStatus QTweetNetBase::variantMapToDirectMessage(const QVariantMap &var)
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

/**
 *  Converts user info
 */
QTweetUser QTweetNetBase::variantMapToUserInfo(const QVariantMap &var)
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

/**
 *  Converts list of user infos
 */
QList<QTweetUser> QTweetNetBase::variantToUserInfoList(const QVariant &fromParser)
{
    QList<QTweetUser> users;

    QList<QVariant> listUsers = fromParser.toList();

    foreach (const QVariant& user, listUsers) {
        QTweetUser userInfo = variantMapToUserInfo(user.toMap());

        users.append(userInfo);
    }

    return users;
}

/**
 *  Converts tweet list
 */
QTweetList QTweetNetBase::variantMapToTweetList(const QVariantMap &var)
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

/**
 *  Converts list of tweet lists
 */
QList<QTweetList> QTweetNetBase::variantToTweetLists(const QVariant &var)
{
    QList<QTweetList> lists;

    QList<QVariant> varLists = var.toList();

    foreach (const QVariant& varlist, varLists) {
        QTweetList tweetlist = variantMapToTweetList(varlist.toMap());

        lists.append(tweetlist);
    }

    return lists;
}

/**
 *  Converts search result
 */
QTweetSearchResult QTweetNetBase::variantMapToSearchResult(const QVariantMap &var)
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

/**
 *  Converts page results
 */
QTweetSearchPageResults QTweetNetBase::variantToSearchPageResults(const QVariant &var)
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

/**
 *  Converts place
 */
QTweetPlace QTweetNetBase::variantMapToPlace(const QVariantMap &var)
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

//not to be used in timelines api, but in geo api, where place contains other places
//is it recursive responsive?
QTweetPlace QTweetNetBase::variantMapToPlaceRecursive(const QVariantMap &var)
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

/**
 *  Convers list of places
 */
QList<QTweetPlace> QTweetNetBase::variantToPlaceList(const QVariant &fromParser)
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
