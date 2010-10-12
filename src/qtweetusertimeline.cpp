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
#include <QNetworkRequest>
#include <QNetworkReply>
#include "qtweetusertimeline.h"
#include "qtweetstatus.h"

QTweetUserTimeline::QTweetUserTimeline(QObject *parent) :
    QTweetNetBase(parent)
{
}

QTweetUserTimeline::QTweetUserTimeline(OAuthTwitter *oauthTwitter, QObject *parent) :
        QTweetNetBase(oauthTwitter, parent)
{
}

/*!
    Starts fetching
    \param userid User ID
    \param screenName User screen name
    \param sinceid Fetches tweets with ID greater (more recent) then sinceid
    \param maxid Fetches tweets with ID less (older) then maxid
    \param count Number of tweets to fetch (up to 200)
    \param page Page number
    \param skipUser True to include only status authors numerical ID
    \param includeRts Timeline contains native retweets if true
    \param includeEntities Each tweet include node "entities"
    \remarks Async
 */
void QTweetUserTimeline::fetch(qint64 userid,
                               const QString &screenName,
                               qint64 sinceid,
                               qint64 maxid,
                               int count,
                               int page,
                               bool trimUser,
                               bool includeRts,
                               bool includeEntities)
{
    QUrl url("http://api.twitter.com/1/statuses/user_timeline.json");

    if (userid != 0)
        url.addQueryItem("user_id", QString::number(userid));

    if (!screenName.isEmpty())
        url.addQueryItem("screen_name", screenName);

    if (sinceid != 0)
        url.addQueryItem("since_id", QString::number(sinceid));

    if (maxid != 0)
        url.addQueryItem("max_id", QString::number(maxid));

    if (count != 0)
        url.addQueryItem("count", QString::number(count));

    if (page != 0)
        url.addQueryItem("page", QString::number(page));

    if (trimUser)
        url.addQueryItem("trim_user", "true");

    if (includeRts)
        url.addQueryItem("include_rts", "true");

    if (includeEntities)
        url.addQueryItem("include_entities", "true");

    QNetworkRequest req(url);

    if (isAuthenticationEnabled()) {
        QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
        req.setRawHeader(AUTH_HEADER, oauthHeader);
    }

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetUserTimeline::parsingJsonFinished(const QVariant &json, bool ok, const QString &errorMsg)
{
    if (ok) {
        QList<QTweetStatus> statuses = variantToStatusList(json);

        emit parsedStatuses(statuses);
    } else {
        qDebug() << "QTweetUserTimeline JSON parser error: " << errorMsg;
        setLastErrorMessage(errorMsg);
        emit error(JsonParsingError, errorMsg);
    }
}
