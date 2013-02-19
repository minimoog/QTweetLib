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
#include "qtweetstatusdestroy.h"
#include "qtweetstatus.h"
#include "qtweetconvert.h"
#include "json/qjsondocument.h"
#include "json/qjsonobject.h"

QTweetStatusDestroy::QTweetStatusDestroy(QObject *parent) :
    QTweetNetBase(parent)
{
}

QTweetStatusDestroy::QTweetStatusDestroy(OAuthTwitter *oauthTwitter, QObject *parent) :
        QTweetNetBase(oauthTwitter, parent)
{
}

/**
 *   Destroys tweet with id
 *   @param id tweet ID
 *   @param trimUser trims users info
 *   @param includeEntities true to include node entities in response
 */
void QTweetStatusDestroy::destroy(qint64 id,
                                  bool trimUser)
{
    if (!isAuthenticationEnabled()) {
        qCritical("Needs authentication to be enabled");
        return;
    }

    QUrl url("https://api.twitter.com/1.1/statuses/destroy.json");

    QUrl urlQuery(url);

    urlQuery.addQueryItem("id", QString::number(id));

    if (trimUser)
        urlQuery.addQueryItem("trim_user", "true");

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(urlQuery, OAuth::POST);
    req.setRawHeader(AUTH_HEADER, oauthHeader);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QByteArray postBody = urlQuery.toEncoded(QUrl::RemoveScheme | QUrl::RemoveAuthority | QUrl::RemovePath);
    postBody.remove(0, 1);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->post(req, postBody);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetStatusDestroy::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isObject()) {
        QTweetStatus status = QTweetConvert::jsonObjectToStatus(jsonDoc.object());

        emit deletedStatus(status);
    }
}

