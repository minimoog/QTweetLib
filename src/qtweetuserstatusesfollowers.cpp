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
#include "qtweetuserstatusesfollowers.h"
#include "qtweetuser.h"
#include "qtweetconvert.h"
#include "json/qjsondocument.h"
#include "json/qjsonarray.h"
#include "json/qjsonobject.h"

QTweetUserStatusesFollowers::QTweetUserStatusesFollowers(QObject *parent) :
        QTweetNetBase(parent), m_usesCursoring(false)
{
}

QTweetUserStatusesFollowers::QTweetUserStatusesFollowers(OAuthTwitter *oauthTwitter, QObject *parent) :
        QTweetNetBase(oauthTwitter, parent), m_usesCursoring(false)
{
}

/**
 *   Starts fetching user followers list
 *   @param userid the ID of the user for whom to return results for.
 *                 Set 0 if you want to fetch followers for authenticaded user
 *   @param cursor breaks the results into pages. Provide a value of "-1" to begin paging.
 *   @param includeEntities when set to true, each tweet will include a node called "entities,".
 */
void QTweetUserStatusesFollowers::fetch(qint64 userid,
                                      const QString &cursor,
                                      bool includeEntities)
{
    QUrl url("http://api.twitter.com/1/statuses/followers.json");

    if (userid != 0)
        url.addQueryItem("user_id", QString::number(userid));

    if (!cursor.isEmpty()) {
        m_usesCursoring = true;
        url.addQueryItem("cursor", cursor);
    }

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

/**
 *   Starts fetching user followers list
 *   @param screenName the screen name of the user for whom to return results for.
 *          Set to empty string if you want to fetch followers for authenticaded user
 *   @param cursor breaks the results into pages. Provide a value of "-1" to begin paging.
 *   @param includeEntities when set to true, each tweet will include a node called "entities,".
 */
void QTweetUserStatusesFollowers::fetch(const QString &screenName,
                                        const QString &cursor,
                                        bool includeEntities)
{
    QUrl url("http://api.twitter.com/1/statuses/followers.json");

    if (!screenName.isEmpty())
        url.addQueryItem("screen_name", screenName);

    if (!cursor.isEmpty()) {
        m_usesCursoring = true;
        url.addQueryItem("cursor", cursor);
    }

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

void QTweetUserStatusesFollowers::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isObject()) {
        if (m_usesCursoring) {
            QJsonObject respJsonObject = jsonDoc.object();

            QJsonArray userListJsonArray = respJsonObject["users"].toArray();

            QList<QTweetUser> userList = QTweetConvert::jsonArrayToUserInfoList(userListJsonArray);

            QString nextCursor = respJsonObject["next_cursor_str"].toString();
            QString prevCursor = respJsonObject["previous_cursor_str"].toString();

            emit parsedFollowersList(userList, nextCursor, prevCursor);
        } else {
            QList<QTweetUser> userList = QTweetConvert::jsonArrayToUserInfoList(jsonDoc.array());

            emit parsedFollowersList(userList);
        }
    }
}
