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
#include "qtweetgeoreversegeocode.h"
#include "qtweetconvert.h"
#include <QJsonDocument>
#include <QJsonObject>

/**
 *  Constructor
 */
QTweetGeoReverseGeoCode::QTweetGeoReverseGeoCode(QObject *parent) :
    QTweetNetBase(parent)
{
}

/**
 *  Constructor
 *  @param oauthTwitter OAuthTwitter object
 *  @param parent parent QObject
 */
QTweetGeoReverseGeoCode::QTweetGeoReverseGeoCode(OAuthTwitter *oauthTwitter, QObject *parent) :
    QTweetNetBase(oauthTwitter, parent)
{
}

/**
 *  Start geo reversing
 *  @param latLong latitutde and longitude
 *  @param accuracy a hint on the "region" in which to search in meters
 *  @param granularity minimal granularity of place types to return
 *  @param maxResults hint as to the number of results to return
 */
void QTweetGeoReverseGeoCode::getPlaces(const QTweetGeoCoord& latLong,
                                        int accuracy,
                                        QTweetPlace::Type granularity,
                                        int maxResults)
{
    QUrl url("http://api.twitter.com/1/geo/reverse_geocode.json");
    QUrlQuery urlQuery;

    urlQuery.addQueryItem("lat", QString::number(latLong.latitude()));
    urlQuery.addQueryItem("long", QString::number(latLong.longitude()));

    if (accuracy != 0)
        urlQuery.addQueryItem("accuracy", QString::number(accuracy));

    switch (granularity) {
    case QTweetPlace::Poi:
        urlQuery.addQueryItem("granularity", "poi");
        break;
    case QTweetPlace::Neighborhood:
        urlQuery.addQueryItem("granularity", "neighborhood");
        break;
    case QTweetPlace::City:
        urlQuery.addQueryItem("granularity", "city");
        break;
    case QTweetPlace::Admin:
        urlQuery.addQueryItem("granularity", "admin");
        break;
    case QTweetPlace::Country:
        urlQuery.addQueryItem("granularity", "country");
        break;
    default:
        ;
    }

    if (maxResults != 0)
        urlQuery.addQueryItem("max_results", QString::number(maxResults));

    url.setQuery(urlQuery);

    QNetworkRequest req(url);

    if (isAuthenticationEnabled()) {
        QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
        req.setRawHeader(AUTH_HEADER, oauthHeader);
    }

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetGeoReverseGeoCode::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isObject()) {
        QList<QTweetPlace> places = QTweetConvert::jsonObjectToPlaceList(jsonDoc.object());

        emit parsedPlaces(places);
    }
}
