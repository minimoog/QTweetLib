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
#include <QThreadPool>
#include "qtweetfriendstimeline.h"
#include "qtweetstatus.h"
#include "qjson/parserrunnable.h"

QTweetFriendsTimeline::QTweetFriendsTimeline(QObject *parent) :
    QTweetNetBase(parent)
{
}

QTweetFriendsTimeline::QTweetFriendsTimeline(OAuthTwitter *oauthTwitter, QObject *parent) :
        QTweetNetBase(oauthTwitter, parent)
{
}

/*!
    Starts fetching friends timeline
    \param sinceid Fetch tweets newer then sinceid
    \param maxid Fetch tweets older then maxid
    \param count Number of tweet to fetch (up to 200)
    \param page Page Number (starts from 1)
    \param skipUser Don't show user info in timeline (only id)
    \param includeRts Timeline contains native retweets if true
    \param includeEntities Each tweet include node "entities"
    \remarks Async
 */
void QTweetFriendsTimeline::fetch(qint64 sinceid,
                                  qint64 maxid,
                                  int count,
                                  int page,
                                  bool trimUser,
                                  bool includeRts,
                                  bool includeEntities)
{
    Q_ASSERT(oauthTwitter() != 0);

    QUrl url("http://api.twitter.com/1/statuses/friends_timeline.json");

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

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error()));
}

void QTweetFriendsTimeline::parsingJsonFinished(const QVariant &json, bool ok, const QString &errorMsg)
{
    if (ok) {
        QList<QTweetStatus> statuses = variantToStatusList(json);

        emit parsedStatuses(statuses);
    } else {
        qDebug() << "QTweetFriendsTimeline JSON Parser error: " << errorMsg;
    }
}
