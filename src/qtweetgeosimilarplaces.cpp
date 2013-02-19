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
#include <QPointF>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "qtweetplace.h"
#include "qtweetgeosimilarplaces.h"
#include "qtweetconvert.h"
#include "json/qjsondocument.h"
#include "json/qjsonobject.h"

/**
 *  Constructor
 */
QTweetGeoSimilarPlaces::QTweetGeoSimilarPlaces(QObject *parent) :
    QTweetNetBase(parent)
{
}

/**
 *  Constructor
 *  @param oauthTwitter OAuthTwitter object
 *  @param parent parent QObject
 */
QTweetGeoSimilarPlaces::QTweetGeoSimilarPlaces(OAuthTwitter *oauthTwitter, QObject *parent) :
    QTweetNetBase(oauthTwitter, parent)
{
}

/**
 *  Start fetching similar places
 *  @param latLong latitude and longitude
 *  @param name the name a place is known as
 *  @param containedWithin this is the placeID which you would like to restrict the search results to
 */
void QTweetGeoSimilarPlaces::get(const QTweetGeoCoord &latLong, const QString &name, const QString &containedWithin)
{
    QUrl url("http://api.twitter.com/1/geo/similar_places.json");

    url.addQueryItem("lat", QString::number(latLong.latitude()));
    url.addQueryItem("long",QString::number(latLong.longitude()));
    url.addEncodedQueryItem("name", QUrl::toPercentEncoding(name));

    if (!containedWithin.isEmpty())
        url.addQueryItem("contained_within", containedWithin);

    QNetworkRequest req(url);

    if (isAuthenticationEnabled()) {
        QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
        req.setRawHeader(AUTH_HEADER, oauthHeader);
    }

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetGeoSimilarPlaces::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isObject()) {
        QList<QTweetPlace> places = QTweetConvert::jsonObjectToPlaceList(jsonDoc.object());

        //get token
        QJsonObject respJsonObject = jsonDoc.object();
        QJsonObject resultJsonObject = respJsonObject["result"].toObject();

        QString token = resultJsonObject["token"].toString();

        emit parsedPlaces(places, token);
    }
}
