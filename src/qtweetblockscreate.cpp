/* Copyright 2011 Antonie Jovanoski
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
#include "qtweetblockscreate.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include "qtweetuser.h"
#include "qtweetconvert.h"
#include "json/qjsondocument.h"
#include "json/qjsonobject.h"

/**
 *  Constructor
 */
QTweetBlocksCreate::QTweetBlocksCreate(QObject *parent) :
    QTweetNetBase(parent)
{
}

/**
 *  Constructor
 *  @param oauthTwitter OAuthTwitter object
 *  @param parent parent QObject
 */
QTweetBlocksCreate::QTweetBlocksCreate(OAuthTwitter *oauthTwitter, QObject *parent) :
    QTweetNetBase(oauthTwitter, parent)
{
}

/**
 *  Blocks user
 *  @param userid id of the user to be blocked
 *  @param includeEntities When set to true, each tweet will include a node called "entities"
 */
void QTweetBlocksCreate::create(qint64 userid, bool includeEntities)
{
    if (!isAuthenticationEnabled()) {
        qCritical("Needs authentication to be enabled");
        return;
    }

    QUrl url("http://api.twitter.com/1/blocks/create.json");

    QUrl urlQuery(url);

    urlQuery.addQueryItem("user_id", QString::number(userid));

    if (includeEntities)
        urlQuery.addQueryItem("include_entities", "true");

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(urlQuery, OAuth::POST);
    req.setRawHeader(AUTH_HEADER, oauthHeader);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QByteArray postBody = urlQuery.toEncoded(QUrl::RemoveScheme | QUrl::RemoveAuthority | QUrl::RemovePath);
    postBody.remove(0, 1);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->post(req, postBody);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

/**
 *  Blocks user
 *  @param screenName screen name of the user to be blocked
 *  @param includeEntities When set to true, each tweet will include a node called "entities"
 */
void QTweetBlocksCreate::create(const QString& screenName, bool includeEntities)
{
    if (!isAuthenticationEnabled()) {
        qCritical("Needs authentication to be enabled");
        return;
    }

    QUrl url("http://api.twitter.com/1/blocks/create.json");

    QUrl urlQuery(url);

    urlQuery.addQueryItem("screen_name", screenName);

    if (includeEntities)
        urlQuery.addQueryItem("include_entities", "true");

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(urlQuery, OAuth::POST);
    req.setRawHeader(AUTH_HEADER, oauthHeader);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QByteArray postBody = urlQuery.toEncoded(QUrl::RemoveScheme | QUrl::RemoveAuthority | QUrl::RemovePath);
    postBody.remove(0, 1);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->post(req, postBody);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetBlocksCreate::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isObject()) {
        QTweetUser user = QTweetConvert::jsonObjectToUser(jsonDoc.object());

        emit finishedCreatingBlock(user);
    }
}
