/* Copyright (c) 2010, Antonie Jovanoski
 *
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact e-mail: Antonie Jovanoski <minimoog77_at_gmail.com>
 */

#include <QtDebug>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QThreadPool>
#include "qtweetuser.h"
#include "qtweetuserstatusesfriends.h"
#include "qtweetconvert.h"
#include "cJSON.h"

QTweetUserStatusesFriends::QTweetUserStatusesFriends(QObject *parent) :
        QTweetNetBase(parent), m_usesCursoring(false)
{
}

QTweetUserStatusesFriends::QTweetUserStatusesFriends(OAuthTwitter *oauthTwitter, QObject *parent) :
        QTweetNetBase(oauthTwitter, parent), m_usesCursoring(false)
{
}

/**
 *   Starts fetching user friends list
 *   @param userid the ID of the user for whom to return results for.
 *   @param cursor breaks the results into pages. Provide a value of "-1" to begin paging.
 *   @param includeEntities when set to true, each tweet will include a node called "entities,".
 */
void QTweetUserStatusesFriends::fetch(qint64 userid,
                                      const QString &cursor,
                                      bool includeEntities)
{
    QUrl url("https://api.twitter.com/1/statuses/friends.json");

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
 *   Starts fetching user friends list
 *   @param screenName the screen name of the user for whom to return results for.
 *   @param cursor breaks the results into pages. Provide a value of "-1" to begin paging.
 *   @param includeEntities when set to true, each tweet will include a node called "entities,".
 */
void QTweetUserStatusesFriends::fetch(const QString &screenName,
                                      const QString &cursor,
                                      bool includeEntities)
{
    QUrl url("http://api.twitter.com/1/statuses/friends.json");

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

void QTweetUserStatusesFriends::parseJsonFinished(cJSON *root)
{
    if (m_usesCursoring) {
        cJSON *userListObject = cJSON_GetObjectItem(root, "users");

        QList<QTweetUser> userList = QTweetConvert::cJSONToUserInfoList(userListObject);

        QString nextCursor = cJSON_GetObjectItem(root, "next_cursor_str")->valuestring;
        QString prevCursor = cJSON_GetObjectItem(root, "previous_cursor_str")->valuestring;

        emit parsedFriendsList(userList, nextCursor, prevCursor);
    } else {
        QList<QTweetUser> userList = QTweetConvert::cJSONToUserInfoList(root);

        emit parsedFriendsList(userList);
    }
}
