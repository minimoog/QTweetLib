/* Copyright 2013 Antonie Jovanoski
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

#include "qtweetretweetsofme.h"
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonArray>
#include "qtweetstatus.h"
#include "qtweetconvert.h"

QTweetRetweetsOfMe::QTweetRetweetsOfMe(QObject *parent) :
    QTweetNetBase(parent),
    m_sinceId(0),
    m_maxId(0),
    m_count(0),
    m_trimUser(false),
    m_includeEntities(false),
    m_includeUserEntities(false)
{
}

QTweetRetweetsOfMe::QTweetRetweetsOfMe(OAuthTwitter *oauthTwitter, QObject *parent) :
    QTweetNetBase(oauthTwitter, parent),
    m_sinceId(0),
    m_maxId(0),
    m_count(0),
    m_trimUser(false),
    m_includeEntities(false),
    m_includeUserEntities(false)
{
}

void QTweetRetweetsOfMe::fetch(qint64 sinceId,
                               qint64 maxId,
                               int count,
                               bool trimUser,
                               bool includeEntities,
                               bool includeUserEntities)
{
    if (!isAuthenticationEnabled()) {
        qCritical("Needs authentication to be enabled");
        return;
    }

    QUrl url("https://api.twitter.com/1.1/statuses/retweets_of_me.json");
    QUrlQuery urlQuery;

    if (sinceId != 0)
        urlQuery.addQueryItem("since_id", QString::number(sinceId));

    if (maxId != 0)
        urlQuery.addQueryItem("max_id", QString::number(maxId));

    if (count != 0)
        urlQuery.addQueryItem("count", QString::number(count));

    if (trimUser)
        urlQuery.addQueryItem("trim_user", "true");

    if (includeEntities)
        urlQuery.addQueryItem("include_entities", "true");

    if (includeUserEntities)
        urlQuery.addQueryItem("include_user_entities", "true");

    url.setQuery(urlQuery);

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetRetweetsOfMe::get()
{
    fetch(m_sinceId, m_maxId, m_count, m_trimUser, m_includeEntities, m_includeUserEntities);
}

void QTweetRetweetsOfMe::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isArray()) {
        QList<QTweetStatus> statuses = QTweetConvert::jsonArrayToStatusList(jsonDoc.array());

        emit parsedStatuses(statuses);
    }
}

