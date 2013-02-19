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
#include "qtweetgeosearch.h"
#include "qtweetconvert.h"
#include "json/qjsondocument.h"
#include "json/qjsonobject.h"

/**
 *  Constructor
 */
QTweetGeoSearch::QTweetGeoSearch(QObject *parent) :
    QTweetNetBase(parent)
{
}

/**
 *  Constructor
 *  @param oauthTwitter OAuthTwitter object
 *  @param parent parent QObject
 */
QTweetGeoSearch::QTweetGeoSearch(OAuthTwitter *oauthTwitter, QObject *parent) :
    QTweetNetBase(oauthTwitter, parent)
{
}

/**
 *  Starts geo searching
 *  @param latLong latitude and longitude
 *  @param query text to match against while executing a geo-based query,
 *               best suited for finding nearby locations by name
 *  @param ip ip address. Used when attempting to fix geolocation based off of the user's IP address.
 *  @param granularity this is the minimal granularity of place types to return
 *  @param accuracy hint on the "region" in which to search in meters
 *  @param maxResults hint as to the number of results to return
 *  @param containedWithin this is the placeID which you would like to restrict the search results to
 */
void QTweetGeoSearch::search(const QTweetGeoCoord &latLong,
                             const QString &query,
                             const QString &ip,
                             QTweetPlace::Type granularity,
                             int accuracy,
                             int maxResults,
                             const QString &containedWithin)
{
    QUrl url("http://api.twitter.com/1/geo/search.json");

    if (latLong.isValid()) {
        url.addQueryItem("lat", QString::number(latLong.latitude()));
        url.addQueryItem("long", QString::number(latLong.longitude()));
    }

    if (!query.isEmpty())
        url.addEncodedQueryItem("query", QUrl::toPercentEncoding(query));

    if (!ip.isEmpty())
        //doesn't do ip format address checking
        url.addQueryItem("ip", ip);

    switch (granularity) {
    case QTweetPlace::Poi:
        url.addQueryItem("granularity", "poi");
        break;
    case QTweetPlace::Neighborhood:
        url.addQueryItem("granularity", "neighborhood");
        break;
    case QTweetPlace::City:
        url.addQueryItem("granularity", "city");
        break;
    case QTweetPlace::Admin:
        url.addQueryItem("granularity", "admin");
        break;
    case QTweetPlace::Country:
        url.addQueryItem("granularity", "country");
        break;
    default:
        ;
    }

    if (accuracy != 0)
        url.addQueryItem("accuracy", QString::number(accuracy));

    if (maxResults != 0)
        url.addQueryItem("max_results", QString::number(maxResults));

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

void QTweetGeoSearch::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isObject()) {
        QList<QTweetPlace> places = QTweetConvert::jsonObjectToPlaceList(jsonDoc.object());

        emit parsedPlaces(places);
    }
}
