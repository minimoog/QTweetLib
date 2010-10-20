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

#include <QNetworkRequest>
#include <QNetworkReply>
#include "qtweetgeosearch.h"

QTweetGeoSearch::QTweetGeoSearch(QObject *parent) :
    QTweetNetBase(parent)
{
}

QTweetGeoSearch::QTweetGeoSearch(OAuthTwitter *oauthTwitter, QObject *parent) :
    QTweetNetBase(oauthTwitter, parent)
{
}

void QTweetGeoSearch::search(QPointF latLong,
                             const QString &query,
                             const QString &ip,
                             QTweetPlace::Type granularity,
                             int accuracy,
                             int maxResults,
                             const QString &containedWithin)
{
    QUrl url("http://api.twitter.com/1/geo/search.json");

    if (!latLong.isNull()) {
        url.addQueryItem("lat", QString::number(latLong.x()));
        url.addQueryItem("long", QString::number(latLong.y()));
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

void QTweetGeoSearch::parsingJsonFinished(const QVariant &json, bool ok, const QString &errorMsg)
{
    if (ok) {
        QList<QTweetPlace> places = variantToPlaceList(json);

        emit parsedPlaces(places);
    } else {
        qDebug() << "QTweetGeoSearch parser error: " << errorMsg;
        setLastErrorMessage(errorMsg);
        emit error(JsonParsingError, errorMsg);
    }
}
