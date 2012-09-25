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
#include "qtweetdirectmessagenew.h"
#include "qtweetdmstatus.h"
#include "qtweetconvert.h"
#include <QJsonDocument>
#include <QJsonObject>

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

    QUrl url("http://api.twitter.com/1/direct_messages/new.json");
    QUrl urlPost(url);
    QUrlQuery urlQuery;

    urlQuery.addQueryItem("user_id", QString::number(user));
    urlQuery.addQueryItem("text", text);

    if (includeEntities)
        urlQuery.addQueryItem("include_entities", "true");

    urlPost.setQuery(urlQuery);

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(urlPost, OAuth::POST);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QByteArray postBody = urlPost.toEncoded(QUrl::RemoveScheme | QUrl::RemoveAuthority | QUrl::RemovePath);
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

    QUrl url("http://api.twitter.com/1/direct_messages/new.json");
    QUrl urlPost(url);
    QUrlQuery urlQuery;

    urlQuery.addQueryItem("screen_name", screenName);
    urlQuery.addQueryItem("text", text);

    if (includeEntities)
        urlQuery.addQueryItem("include_entities", "true");

    urlPost.setQuery(urlQuery);

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(urlPost, OAuth::POST);
    req.setRawHeader(AUTH_HEADER, oauthHeader);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QByteArray postBody = urlPost.toEncoded(QUrl::RemoveScheme | QUrl::RemoveAuthority | QUrl::RemovePath);
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
