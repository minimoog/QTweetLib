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

#include <QNetworkReply>
#include <QNetworkRequest>
#include "qtweetretweettome.h"
#include "qtweetstatus.h"

QTweetRetweetToMe::QTweetRetweetToMe(QObject *parent) :
    QTweetNetBase(parent)
{
}

QTweetRetweetToMe::QTweetRetweetToMe(OAuthTwitter *oauthTwitter, QObject *parent) :
    QTweetNetBase(oauthTwitter, parent)
{
}

/*!
    Starts fetching
    \param respType Response type
    \param sinceid Fetches tweets with ID greater (more recent) then sinceid
    \param maxid Fetches tweets with ID less (older) then maxid
    \param count Number of tweets to fetch (up to 200)
    \param page Page number
    \remarks Async
 */
void QTweetRetweetToMe::fetch(ResponseType respType,
                                qint64 sinceid,
                                qint64 maxid,
                                int count,
                                int page)
{
    Q_ASSERT(oauthTwitter() != 0);

    QUrl url;

    if (respType == QTweetNetBase::JSON)
        url.setUrl("http://api.twitter.com/1/statuses/retweeted_to_me.json");
    else
        url.setUrl("http://api.twitter.com/1/statuses/retweeted_to_me.xml");

    if (sinceid != 0)
        url.addQueryItem("since_id", QString::number(sinceid));

    if (maxid != 0)
        url.addQueryItem("max_id", QString::number(maxid));

    if (count != 0)
        url.addQueryItem("count", QString::number(count));

    if (page != 0)
        url.addQueryItem("page", QString::number(page));

    // ### TODO Add trim_user parameter
    // ### TODO Add include_entities parameter

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error()));
}

void QTweetRetweetToMe::reply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (reply) {
         m_response = reply->readAll();
        emit finished(m_response);

        reply->deleteLater();

        if (isJsonParsingEnabled())
            parseJson(m_response);
    }
}

void QTweetRetweetToMe::parsingJsonFinished(const QVariant &json, bool ok, const QString &errorMsg)
{
    if (ok) {
        QList<QTweetStatus> statuses = variantToStatusList(json);

        emit parsedStatuses(statuses);
    } else {
        qDebug() << "JSON parser error: " << errorMsg;
    }
}

void QTweetRetweetToMe::error()
{
    // ### TODO:
}
