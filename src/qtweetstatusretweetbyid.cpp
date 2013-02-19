/* Copyright 2012 Antonie Jovanoski
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

#include "qtweetstatusretweetbyid.h"
#include <QtDebug>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "qtweetconvert.h"
#include "json/qjsondocument.h"
#include "json/qjsonarray.h"

QTweetStatusRetweetByID::QTweetStatusRetweetByID(QObject *parent) :
    QTweetNetBase(parent),
    m_tweetid(0),
    m_count(0),
    m_page(0)
{
}

QTweetStatusRetweetByID::QTweetStatusRetweetByID(OAuthTwitter *oauthTwitter, QObject *parent) :
    QTweetNetBase(oauthTwitter, parent),
    m_tweetid(0),
    m_count(0),
    m_page(0)
{
}

void QTweetStatusRetweetByID::fetch(qint64 tweetid, int count, int page)
{
    if (!isAuthenticationEnabled()) {
        qCritical("Needs authentication to be enabled");
        return;
    }

    QString urlString = QString("https://api.twitter.com/1/statuses/%1/retweeted_by/ids.json").arg(tweetid);
    QUrl url(urlString);

    if (count != 0)
        url.addQueryItem("count", QString::number(count));

    if (page != 0)
        url.addQueryItem("page", QString::number(page));

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetStatusRetweetByID::get()
{
    fetch(m_tweetid, m_count, m_page);
}

void QTweetStatusRetweetByID::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isArray()) {
        QList<qint64> userid;
        QJsonArray idJsonArray = jsonDoc.array();

        for (int i = 0; i < idJsonArray.size(); ++i)
            userid.append(static_cast<qint64>(idJsonArray[i].toDouble()));

        emit parsedUsersID(userid);
    }
}
