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
#include "qtweetstatusupdate.h"
#include "qtweetstatus.h"
#include "qtweetgeocoord.h"
#include "qtweetconvert.h"
#include "json/qjsondocument.h"
#include "json/qjsonobject.h"

QTweetStatusUpdate::QTweetStatusUpdate(QObject *parent) :
    QTweetNetBase(parent)
{
}

QTweetStatusUpdate::QTweetStatusUpdate(OAuthTwitter *oauthTwitter, QObject *parent) :
        QTweetNetBase(oauthTwitter, parent)
{
}

/**
 *   Posts a tweet
 *   @param status text of the status update
 *   @param inReplyToStatus ID of a existing tweet is in reply to
 *   @param latLong latitude and longitude
 *   @param placeid a place in the world (use reverse geocoding)
 *   @param displayCoordinates whether or not to put a exact coordinates a tweet has been sent from
 */
void QTweetStatusUpdate::post(const QString &status,
                              qint64 inReplyToStatus,
                              const QTweetGeoCoord& latLong,
                              const QString &placeid,
                              bool displayCoordinates,
                              bool trimUser,
                              bool includeEntities)
{
    if (!isAuthenticationEnabled()) {
        qCritical("Needs authentication to be enabled");
        return;
    }

    QUrl url("https://api.twitter.com/1.1/statuses/update.json");

    QUrl urlQuery("https://api.twitter.com/1.1/statuses/update.json");

    urlQuery.addEncodedQueryItem("status", QUrl::toPercentEncoding(status));

    if (inReplyToStatus != 0)
        urlQuery.addQueryItem("in_reply_to_status_id", QString::number(inReplyToStatus));

    if (latLong.isValid()) {
        urlQuery.addQueryItem("lat", QString::number(latLong.latitude()));
        urlQuery.addQueryItem("long", QString::number(latLong.longitude()));
    }

    if (!placeid.isEmpty())
        urlQuery.addQueryItem("place_id", placeid);

    if (displayCoordinates)
        urlQuery.addQueryItem("display_coordinates", "true");

    if (trimUser)
        urlQuery.addQueryItem("trim_user", "true");

    if (includeEntities)
        urlQuery.addQueryItem("include_entities", "true");

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(urlQuery, OAuth::POST);
    QNetworkRequest req(url);
    req.setRawHeader(AUTH_HEADER, oauthHeader);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    //build status post array
    QByteArray statusPost = urlQuery.toEncoded(QUrl::RemoveScheme | QUrl::RemoveAuthority | QUrl::RemovePath);

    //remove '?'
    statusPost.remove(0, 1);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->post(req, statusPost);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetStatusUpdate::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isObject()) {
        QTweetStatus status = QTweetConvert::jsonObjectToStatus(jsonDoc.object());

        emit postedStatus(status);
    }
}

