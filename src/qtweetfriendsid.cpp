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
#include <QNetworkRequest>
#include <QNetworkReply>
#include "qtweetfriendsid.h"
#include "json/qjsondocument.h"
#include "json/qjsonarray.h"
#include "json/qjsonobject.h"

/**
 *  Constructor
 */
QTweetFriendsID::QTweetFriendsID(QObject *parent) :
    QTweetNetBase(parent)
{
}

/**
 *  Constructor
 *  @param oauthTwitter OAuthTwitter object
 *  @param parent parent QObject
 */
QTweetFriendsID::QTweetFriendsID(OAuthTwitter *oauthTwitter, QObject *parent) :
        QTweetNetBase(oauthTwitter, parent)
{
}

/**
 *  Start fetcing one page of id's
 *  @param user the ID of the user for whom to return results for.
 *  @param cursor use from response nextCursor and prevCursor to allow paging back and forth
 */
void QTweetFriendsID::fetch(qint64 user, const QString &cursor)
{
    QUrl url("http://api.twitter.com/1/friends/ids.json");

    url.addQueryItem("user_id", QString::number(user));
    url.addQueryItem("cursor", cursor);

    QNetworkRequest req(url);

    if (isAuthenticationEnabled()) {
        QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
        req.setRawHeader(AUTH_HEADER, oauthHeader);
    }

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

/**
 *  Start fetcing one page of id's
 *  @param screenName the screen name of the user for whom to return results for.
 *  @param cursor use from response nextCursor and prevCursor to allow paging back and forth
 */
void QTweetFriendsID::fetch(const QString &screenName, const QString &cursor)
{
    QUrl url("http://api.twitter.com/1/friends/ids.json");

    url.addQueryItem("screen_name", screenName);
    url.addQueryItem("cursor", cursor);

    QNetworkRequest req(url);

    if (isAuthenticationEnabled()) {
        QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
        req.setRawHeader(AUTH_HEADER, oauthHeader);
    }

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetFriendsID::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isObject()) {
        QList<qint64> idList;

        QJsonObject respJsonObject = jsonDoc.object();

        QJsonArray idJsonArray = respJsonObject["ids"].toArray();

        for (int i = 0; i < idJsonArray.size(); ++i)
            idList.append(static_cast<qint64>(idJsonArray[i].toDouble()));

        QString nextCursor = respJsonObject["next_cursor_str"].toString();
        QString prevCursor = respJsonObject["previous_cursor_str"].toString();

        emit parsedIDs(idList, nextCursor, prevCursor);
    }
}
