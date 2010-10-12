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

#include <QThreadPool>
#include <QNetworkReply>
#include "qtweetnetbase.h"
#include "qtweetstatus.h"
#include "qtweetdmstatus.h"
#include "qtweetuser.h"
#include "qtweetlist.h"
#include "qtweetsearchresult.h"
#include "qtweetsearchpageresults.h"
#include "qjson/parserrunnable.h"
#include "qjson/parser.h"

/*!
    Constructor
 */
QTweetNetBase::QTweetNetBase(QObject *parent) :
    QObject(parent), m_oauthTwitter(0)
{
}

/*!
    Constructor
    \param oauthTwitter OAuth Twitter
    \param parent QObject parent
 */
QTweetNetBase::QTweetNetBase(OAuthTwitter *oauthTwitter, QObject *parent) :
        QObject(parent), m_oauthTwitter(oauthTwitter)
{

}

QTweetNetBase::~QTweetNetBase()
{
}

/*!
    Sets OAuth Twitter authorization
    \param oauthTwitter OAuth Twitter
 */
void QTweetNetBase::setOAuthTwitter(OAuthTwitter *oauthTwitter)
{
    m_oauthTwitter = oauthTwitter;
}

/*!
    Gets OAuth Twitter authorization
    \return OAuth Twitter
 */
OAuthTwitter* QTweetNetBase::oauthTwitter() const
{
    return m_oauthTwitter;
}

QByteArray QTweetNetBase::response() const
{
    return m_response;
}

QString QTweetNetBase::lastErrorMessage() const
{
    return m_lastErrorMessage;
}

void QTweetNetBase::setJsonParsingEnabled(bool enable)
{
    m_jsonParsingEnabled = enable;
}

bool QTweetNetBase::isJsonParsingEnabled() const
{
    return m_jsonParsingEnabled;
}

void QTweetNetBase::parseJson(const QByteArray &jsonData)
{
    QJson::ParserRunnable *jsonParser = new QJson::ParserRunnable;
    jsonParser->setData(jsonData);

    connect(jsonParser, SIGNAL(parsingFinished(QVariant,bool,QString)),
            this, SLOT(parsingJsonFinished(QVariant,bool,QString)));

    QThreadPool::globalInstance()->start(jsonParser);
}

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
            case NotModified:   /*!< There was no new data to return. */
            case BadRequest:    /*!< The request was invalid. This is the status code will be returned during rate limiting. */
            case Unauthorized:  /*!< Authentication credentials were missing or incorrect. */
            case Forbidden:     /*!< The request is understood, but it has been refused, due to update limits. */
            case NotFound:      /*!< The URI requested is invalid or the resource requested, such as a user, does not exists. */
            case NotAcceptable: /*!< Returned by the Search API when an invalid format is specified in the request. */
            case EnhanceYourCalm:   /*!< Returned by the Search and Trends API when you are being rate limited. */
            case InternalServerError:   /*!< Something is broken in Twitter */
            case BadGateway:    /*!< Twitter is down or being upgraded. */
            case ServiceUnavailable:    /*! The Twitter servers are up, but overloaded with requests. Try again later. */
                emit error(static_cast<ErrorCode>(httpStatus), m_lastErrorMessage);
                break;
            default:
                emit error(UnknownError, m_lastErrorMessage);
            }
        }
        reply->deleteLater();
    }
}

void QTweetNetBase::setLastErrorMessage(const QString &errMsg)
{
    m_lastErrorMessage = errMsg;
}

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

    return status;
}

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
