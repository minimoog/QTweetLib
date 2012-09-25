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
#include <QUrlQuery>
#include "qtweetuserstatusesfollowers.h"
#include "qtweetuser.h"
#include "qtweetconvert.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

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
    QUrlQuery urlQuery;

    if (userid != 0)
        urlQuery.addQueryItem("user_id", QString::number(userid));

    if (!cursor.isEmpty()) {
        m_usesCursoring = true;
        urlQuery.addQueryItem("cursor", cursor);
    }

    if (includeEntities)
        urlQuery.addQueryItem("include_entities", "true");

    url.setQuery(urlQuery);

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
    QUrlQuery urlQuery;

    if (!screenName.isEmpty())
        urlQuery.addQueryItem("screen_name", screenName);

    if (!cursor.isEmpty()) {
        m_usesCursoring = true;
        urlQuery.addQueryItem("cursor", cursor);
    }

    if (includeEntities)
        urlQuery.addQueryItem("include_entities", "true");

    url.setQuery(urlQuery);

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
