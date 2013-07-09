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
#include "qtweetdirectmessagenew.h"
#include "qtweetdmstatus.h"
#include "qtweetconvert.h"
#include "json/qjsondocument.h"
#include "json/qjsonobject.h"

/**
 *  Constructor
 */
QTweetDirectMessageNew::QTweetDirectMessageNew(QObject *parent) :
    QTweetNetBase(parent)
{
}

/**
 *  Constructor
 *  @param oauthTwitter OAuthTwitter object
 *  @param parent parent QObject
 */
QTweetDirectMessageNew::QTweetDirectMessageNew(OAuthTwitter *oauthTwitter, QObject *parent) :
        QTweetNetBase(oauthTwitter, parent)
{
}

/**
 *   Sends direct message
 *   @param user The ID of the user who should receive the direct message.
 *   @param text The text of direct message
 *   @param includeEntities When set to true each tweet will include a node called "entities,"
 */
void QTweetDirectMessageNew::post(qint64 user,
                                  const QString &text,
                                  bool includeEntities)
{
    if (!isAuthenticationEnabled()) {
        qCritical("Needs authentication to be enabled");
        return;
    }

    QUrl url("http://api.twitter.com/1.1/direct_messages/new.json");

    QUrl urlQuery(url);

    urlQuery.addQueryItem("user_id", QString::number(user));
    urlQuery.addEncodedQueryItem("text", QUrl::toPercentEncoding(text));

    if (includeEntities)
        urlQuery.addQueryItem("include_entities", "true");

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(urlQuery, OAuth::POST);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QByteArray postBody = urlQuery.toEncoded(QUrl::RemoveScheme | QUrl::RemoveAuthority | QUrl::RemovePath);
    postBody.remove(0, 1);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->post(req, postBody);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

/**
 *   Sends direct message
 *   @param user The ID of the user who should receive the direct message.
 *   @param text The text of direct message
 *   @param includeEntities When set to true each tweet will include a node called "entities,"
 */
void QTweetDirectMessageNew::post(const QString &screenName, const QString &text, bool includeEntities)
{
    if (!isAuthenticationEnabled()) {
        qCritical("Needs authentication to be enabled");
        return;
    }

    QUrl url("http://api.twitter.com/1.1/direct_messages/new.json");

    QUrl urlQuery(url);

    urlQuery.addEncodedQueryItem("screen_name", QUrl::toPercentEncoding(screenName));
    urlQuery.addEncodedQueryItem("text", QUrl::toPercentEncoding(text));

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

void QTweetDirectMessageNew::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isObject()) {
        QTweetDMStatus dm = QTweetConvert::jsonObjectToDirectMessage(jsonDoc.object());

        emit parsedDirectMessage(dm);
    }
}
