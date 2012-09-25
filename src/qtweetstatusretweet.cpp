/* Copyright (c) 2010, Antonie Jovanoski
 *
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact e-mail: Antonie Jovanoski <minimoog77_at_gmail.com>
 */

#include <QtDebug>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>
#include "qtweetstatusretweet.h"
#include "qtweetstatus.h"
#include "qtweetconvert.h"
#include <QJsonDocument>
#include <QJsonObject>

QTweetStatusRetweet::QTweetStatusRetweet(QObject *parent) :
    QTweetNetBase(parent)
{
}

QTweetStatusRetweet::QTweetStatusRetweet(OAuthTwitter *oauthTwitter, QObject *parent) :
        QTweetNetBase(oauthTwitter, parent)
{
}

/**
 *   Retweets the tweet
 *   @param id tweet ID to retweet
 *   @param trimUser trims user info in response
 *   @param includeEntities true to include node entities in response
 */
void QTweetStatusRetweet::retweet(qint64 id,
                                  bool trimUser,
                                  bool includeEntities)
{
    QUrl url(QString("https://api.twitter.com/1.1/statuses/retweet/%1.json").arg(id));
    QUrlQuery urlQuery;

    if (trimUser)
        urlQuery.addQueryItem("trim_user", "true");

    if (includeEntities)
        urlQuery.addQueryItem("include_entities", "true");

    url.setQuery(urlQuery);

    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    if (isAuthenticationEnabled()) {    //Oh, Twitter API docs wrong?
        QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::POST);
        req.setRawHeader(AUTH_HEADER, oauthHeader);
    }

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->post(req, QByteArray());
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetStatusRetweet::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isObject()) {
        QTweetStatus status = QTweetConvert::jsonObjectToStatus(jsonDoc.object());

        emit postedRetweet(status);
    }
}

