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
#include "qtweetdirectmessagedestroy.h"
#include "qtweetdmstatus.h"
#include "qtweetconvert.h"
#include "json/qjsondocument.h"
#include "json/qjsonobject.h"

/**
 *  Constructor
 */
QTweetDirectMessageDestroy::QTweetDirectMessageDestroy(QObject *parent) :
    QTweetNetBase(parent)
{
}

/**
 *  Constructor
 *  @param oauthTwitter OAuthTwitter object
 *  @param parent parent QObject
 */
QTweetDirectMessageDestroy::QTweetDirectMessageDestroy(OAuthTwitter *oauthTwitter, QObject *parent) :
        QTweetNetBase(oauthTwitter, parent)
{
}

/**
 *   @param id the ID of the direct message to delete.
 *   @param includeEntities When set to true, each tweet will include a node called "entities,"
 */
void QTweetDirectMessageDestroy::destroyMessage(qint64 id, bool includeEntities)
{
    if (!isAuthenticationEnabled()) {
        qCritical("Needs authentication to be enabled");
        return;
    }

    QUrl url("https://api.twitter.com/1.1/direct_messages/destroy.json");
    QUrl urlQuery("https://api.twitter.com/1.1/direct_messages/destroy.json");

    urlQuery.addQueryItem("id", QString::number(id));

    if (includeEntities)
        urlQuery.addQueryItem("include_entities", "true");

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(urlQuery, OAuth::POST);
    req.setRawHeader(AUTH_HEADER, oauthHeader);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QByteArray statusPost = urlQuery.toEncoded(QUrl::RemoveScheme | QUrl::RemoveAuthority | QUrl::RemovePath);
    statusPost.remove(0, 1);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->post(req, statusPost);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetDirectMessageDestroy::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isObject()) {
        QTweetDMStatus dm = QTweetConvert::jsonObjectToDirectMessage(jsonDoc.object());

        emit parsedDirectMessage(dm);
    }
}

