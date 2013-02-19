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
#include "qtweetfriendshipdestroy.h"
#include "qtweetuser.h"
#include "qtweetconvert.h"
#include "json/qjsondocument.h"
#include "json/qjsonobject.h"

/**
 *  Constructor
 */
QTweetFriendshipDestroy::QTweetFriendshipDestroy(QObject *parent) :
    QTweetNetBase(parent)
{
}

/**
 *  Constructor
 *  @param oauthTwitter OAuthTwitter object
 *  @param parent parent QObject
 */
QTweetFriendshipDestroy::QTweetFriendshipDestroy(OAuthTwitter *oauthTwitter, QObject *parent) :
        QTweetNetBase(oauthTwitter, parent)
{
}

/**
 *  Unfollows the specified user
 *  @param userid user id to unfollow
 *  @param includeEntities when set totrue, each tweet will include a node called "entities,".
 */
void QTweetFriendshipDestroy::unfollow(qint64 userid, bool includeEntities)
{
    if (!isAuthenticationEnabled()) {
        qCritical("Needs authentication to be enabled");
        return;
    }

    QUrl url("http://api.twitter.com/1/friendships/destroy.json");

    url.addQueryItem("user_id", QString::number(userid));

    if (includeEntities)
        url.addQueryItem("include_entities", "true");

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::DELETE);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->deleteResource(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

/**
 *  Unfollows the specified user
 *  @param screenName screen name to unfollow
 *  @param includeEntities when set totrue, each tweet will include a node called "entities,".
 */
void QTweetFriendshipDestroy::unfollow(const QString &screenName, bool includeEntities)
{
    if (!isAuthenticationEnabled()) {
        qCritical("Needs authentication to be enabled");
        return;
    }

    QUrl url("http://api.twitter.com/1/friendships/destroy.json");

    url.addQueryItem("screen_name", screenName);

    if (includeEntities)
        url.addQueryItem("include_entities", "true");

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::DELETE);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->deleteResource(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetFriendshipDestroy::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isObject()) {
        QTweetUser user = QTweetConvert::jsonObjectToUser(jsonDoc.object());

        emit parsedUser(user);
    }
}
