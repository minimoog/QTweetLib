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
#include "qtweetnetbase.h"
#include "qtweetstatus.h"
#include "qtweetdmstatus.h"
#include "qtweetuser.h"
#include "qjson/parserrunnable.h"

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

void QTweetNetBase::parsingJsonFinished(const QVariant &json, bool ok, const QString &errorMsg)
{
    // ### TODO: Make this method abstract
}

QList<QTweetStatus> QTweetNetBase::variantToStatusList(const QVariant &fromParser)
{
    QList<QTweetStatus> statuses;

    QList<QVariant> listStatus = fromParser.toList();

    // ### TODO: Parsing when user info is trimmed

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
        QTweetDMStatus dmStatus;

        QVariantMap dmMap = message.toMap();

        dmStatus.setCreatedAt(dmMap["created_at"].toString());
        dmStatus.setSenderScreenName(dmMap["sender_screen_name"].toString());

        QVariantMap senderMap = dmMap["sender"].toMap();
        QTweetUser sender = variantMapToUserInfo(senderMap);

        dmStatus.setSender(sender);

        dmStatus.setText(dmMap["text"].toString());
        dmStatus.setRecipientScreenName(dmMap["recipient_screen_name"].toString());
        dmStatus.setId(dmMap["id"].toLongLong());

        QVariantMap recipientMap = dmMap["recipient"].toMap();
        QTweetUser recipient = variantMapToUserInfo(recipientMap);

        dmStatus.setRecipient(recipient);

        dmStatus.setRecipientId(dmMap["recipient_id"].toLongLong());
        dmStatus.setSenderId(dmMap["sender_id"].toLongLong());

        directMessages.append(dmStatus);
    }
    return directMessages;
}

QTweetUser QTweetNetBase::variantMapToUserInfo(const QVariantMap &var)
{
    QTweetUser userInfo;

    userInfo.setName(var["name"].toString());
    userInfo.setLocation(var["location"].toString());
    userInfo.setprofileImageUrl(var["profile_image_url"].toString());
    userInfo.setCreatedAt(var["created_at"].toString());
    userInfo.setFavouritesCount(var["favourites_count"].toInt());
    userInfo.setUrl(var["url"].toString());
    userInfo.setUtcOffset(var["utc_offset"].toInt());
    userInfo.setId(var["id"].toLongLong());
    userInfo.setProtected(var["protected"].toBool());
    userInfo.setFollowersCount(var["followers_count"].toInt());
    userInfo.setVerified(var["verified"].toBool());
    userInfo.setGeoEnabled(var["geo_enabled"].toBool());
    userInfo.setDescription(var["description"].toString());
    userInfo.setTimezone(var["time_zone"].toString());
    userInfo.setFriendsCount(var["friends_count"].toInt());
    userInfo.setStatusesCount(var["statuses_count"].toInt());
    userInfo.setScreenName(var["screen_name"].toString());
    userInfo.setFollowing(var["following"].toBool());

    //check if contains status
    if (var.contains("status")) {
        QVariantMap statusMap = var["status"].toMap();

        QTweetStatus status = variantMapToStatus(var);

        userInfo.setStatus(status);
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
