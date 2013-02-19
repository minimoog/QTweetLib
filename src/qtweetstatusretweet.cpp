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
#include "qtweetstatusretweet.h"
#include "qtweetstatus.h"
#include "qtweetconvert.h"
#include "json/qjsondocument.h"
#include "json/qjsonobject.h"

QTweetStatusRetweet::QTweetStatusRetweet(QObject *parent) :
    QTweetNetBase(parent)
{
}

QTweetStatusRetweet::QTweetStatusRetweet(OAuthTwitter *oauthTwitter, QObject *parent) :
        QTweetNetBase(oauthTwitter, parent)
{
}

/**
 *   Retweets the tweet
 *   @param id tweet ID to retweet
 *   @param trimUser trims user info in response
 *   @param includeEntities true to include node entities in response
 */
void QTweetStatusRetweet::retweet(qint64 id,
                                  bool trimUser,
                                  bool includeEntities)
{
    QUrl url(QString("https://api.twitter.com/1.1/statuses/retweet/%1.json").arg(id));

    if (trimUser)
        url.addQueryItem("trim_user", "true");

    if (includeEntities)
        url.addQueryItem("include_entities", "true");

    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    if (isAuthenticationEnabled()) {    //Oh, Twitter API docs wrong?
        QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::POST);
        req.setRawHeader(AUTH_HEADER, oauthHeader);
    }

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->post(req, QByteArray());
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetStatusRetweet::retweet()
{
    retweet(m_id, m_trimUser, m_includeEntities);
}

void QTweetStatusRetweet::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isObject()) {
        QTweetStatus status = QTweetConvert::jsonObjectToStatus(jsonDoc.object());

        emit postedRetweet(status);
    }
}

