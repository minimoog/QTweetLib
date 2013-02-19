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

#include <QtDebug>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "qtweetstatusshow.h"
#include "qtweetstatus.h"
#include "qtweetconvert.h"
#include "json/qjsondocument.h"
#include "json/qjsonobject.h"

QTweetStatusShow::QTweetStatusShow(QObject *parent) :
    QTweetNetBase(parent),
    m_tweetid(0),
    m_includeMyRetweet(false),
    m_includeEntities(false)
{
}

QTweetStatusShow::QTweetStatusShow(OAuthTwitter *oauthTwitter, QObject *parent) :
    QTweetNetBase(oauthTwitter, parent),
    m_tweetid(0),
    m_includeMyRetweet(false),
    m_includeEntities(false)
{
}

/**
 *   Starts fetching
 *   @param id tweet ID
 *   @param trimUser set to true to trim user info in response
 *   @param includeEntities set to true to include node entities in response
 */
void QTweetStatusShow::fetch(qint64 id, bool trimUser, bool includeMyRetweet, bool includeEntities)
{
    QUrl url("https://api.twitter.com/1.1/statuses/show.json");

    url.addQueryItem("id", QString::number(id));

    if (trimUser)
        url.addQueryItem("trim_user", "true");

    if (includeMyRetweet)
        url.addQueryItem("include_my_retweet", "true");

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

void QTweetStatusShow::get()
{
    fetch(m_tweetid, m_trimUser, m_includeMyRetweet, m_includeEntities);
}

void QTweetStatusShow::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isObject()) {
        QTweetStatus status = QTweetConvert::jsonObjectToStatus(jsonDoc.object());

        emit parsedStatus(status);
    }
}
