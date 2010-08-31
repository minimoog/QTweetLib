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
        QTweetStatus tweetStatus;

        QVariantMap statusMap = status.toMap();

        tweetStatus.setId(statusMap["id"].toLongLong());
        tweetStatus.setText(statusMap["text"].toString());
        tweetStatus.setSource(statusMap["source"].toString());
        tweetStatus.setInReplyToStatusId(statusMap["in_reply_to_status_id"].toLongLong());
        tweetStatus.setInReplyToUserId(statusMap["in_reply_to_user_id"].toLongLong());
        tweetStatus.setInReplyToScreenName(statusMap["in_reply_to_screen_name"].toString());

        // ### TODO: Parsing native retweets

        QTweetUser user;

        QVariantMap userMap = statusMap["user"].toMap();

        user.setId(userMap["id"].toLongLong());
        user.setName(userMap["name"].toString());
        user.setScreenName(userMap["screen_name"].toString());
        user.setprofileImageUrl(userMap["profile_image_url"].toString());

        tweetStatus.setUser(user);

        statuses.append(tweetStatus);
    }
    return statuses;
}
