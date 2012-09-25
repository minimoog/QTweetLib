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
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonArray>
#include "qtweetstatusretweets.h"
#include "qtweetstatus.h"
#include "qtweetconvert.h"

QTweetStatusRetweets::QTweetStatusRetweets(QObject *parent) :
    QTweetNetBase(parent),
    m_tweetid(0),
    m_count(0),
    m_trimUser(false)
{
}

QTweetStatusRetweets::QTweetStatusRetweets(OAuthTwitter *oauthTwitter, QObject *parent) :
    QTweetNetBase(oauthTwitter, parent),
    m_tweetid(0),
    m_count(0),
    m_trimUser(false)
{
}

/**
 *   Start fetching
 *   @param id tweet ID
 *   @param count numbers of retweets to fetch
 */
void QTweetStatusRetweets::fetch(qint64 id, int count, bool trimUser)
{
    QString urlString = QString("https://api.twitter.com/1.1/statuses/retweets/%1.json").arg(id);
    QUrl url(urlString);
    QUrlQuery urlQuery;

    if (count != 0)
        urlQuery.addQueryItem("count", QString::number(count));

    if (trimUser)
        urlQuery.addQueryItem("trim_user", "true");

    url.setQuery(urlQuery);

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetStatusRetweets::get()
{
    fetch(m_tweetid, m_count, m_trimUser);
}

void QTweetStatusRetweets::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isArray()) {
        QList<QTweetStatus> statuses = QTweetConvert::jsonArrayToStatusList(jsonDoc.array());

        emit parsedStatuses(statuses);
    }
}

