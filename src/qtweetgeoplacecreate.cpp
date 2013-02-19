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
#include "qtweetgeoplacecreate.h"
#include "qtweetplace.h"
#include "qtweetconvert.h"
#include "json/qjsondocument.h"
#include "json/qjsonobject.h"

/**
 *  Constructor
 */
QTweetGeoPlaceCreate::QTweetGeoPlaceCreate(QObject *parent) :
    QTweetNetBase(parent)
{
}

/**
 *  Constructor
 *  @param oauthTwitter OAuthTwitter object
 *  @param parent parent QObject
 */
QTweetGeoPlaceCreate::QTweetGeoPlaceCreate(OAuthTwitter *oauthTwitter, QObject *parent) :
    QTweetNetBase(oauthTwitter, parent)
{
}

/**
 *  Creates place
 *  @param name the name a place is known as
 *  @param containedWithin placeid within which the new place can be found. Be close as possible with
                           contained place
 *  @param token token found in the response from QTweetGeoSimilarPlaces
 *  @param latLong latitude and longitude
 */
void QTweetGeoPlaceCreate::create(const QString &name,
                                  const QString &containedWithin,
                                  const QString &token,
                                  const QTweetGeoCoord &latLong)
{
    if (!isAuthenticationEnabled()) {
        qCritical("Needs authentication to be enabled");
        return;
    }

    QUrl url("http://api.twitter.com/1/geo/place.json");
    QUrl urlQuery(url);

    urlQuery.addEncodedQueryItem("name", QUrl::toPercentEncoding(name));
    urlQuery.addQueryItem("contained_within", containedWithin);
    urlQuery.addQueryItem("token", token);
    urlQuery.addQueryItem("lat", QString::number(latLong.latitude()));
    urlQuery.addQueryItem("long", QString::number(latLong.longitude()));

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(urlQuery, OAuth::POST);

    QNetworkRequest req(url);
    req.setRawHeader(AUTH_HEADER, oauthHeader);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QByteArray statusPost = urlQuery.toEncoded(QUrl::RemoveScheme | QUrl::RemoveAuthority | QUrl::RemovePath);
    statusPost.remove(0, 1);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->post(req, statusPost);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetGeoPlaceCreate::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isObject()) {
        QTweetPlace place = QTweetConvert::jsonObjectToPlaceRecursive(jsonDoc.object());

        emit parsedPlace(place);
    }
}
