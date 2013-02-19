/* Copyright 2010 Antonie Jovanoski
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Contact e-mail: Antonie Jovanoski <minimoog77_at_gmail.com>
 */

#include <QNetworkReply>
#include <QNetworkRequest>
#include "json/qjsondocument.h"
#include "json/qjsonarray.h"
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

    if (count != 0)
        url.addQueryItem("count", QString::number(count));

    if (trimUser)
        url.addQueryItem("trim_user", "true");

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

