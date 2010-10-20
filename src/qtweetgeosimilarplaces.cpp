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
#include "qtweetplace.h"
#include "qtweetgeosimilarplaces.h"

QTweetGeoSimilarPlaces::QTweetGeoSimilarPlaces(QObject *parent) :
    QTweetNetBase(parent)
{
}

QTweetGeoSimilarPlaces::QTweetGeoSimilarPlaces(OAuthTwitter *oauthTwitter, QObject *parent) :
    QTweetNetBase(oauthTwitter, parent)
{
}

void QTweetGeoSimilarPlaces::get(const QPointF &latLong, const QString &name, const QString &containedWithin)
{
    QUrl url("http://api.twitter.com/1/geo/similar_places.json");

    url.addQueryItem("lat", QString::number(latLong.x()));
    url.addQueryItem("long",QString::number(latLong.y()));
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

void QTweetGeoSimilarPlaces::parsingJsonFinished(const QVariant &json, bool ok, const QString &errorMsg)
{
    if (ok) {
        QList<QTweetPlace> places = variantToPlaceList(json);

        //get token
        QVariantMap respMap = json.toMap();
        QVariantMap resultMap = respMap["result"].toMap();

        QString token = resultMap["token"].toString();

        emit parsedPlaces(places, token);
    } else {
        qDebug() << "QTweetGeoSimilarPlaces parser error: " << errorMsg;
        setLastErrorMessage(errorMsg);
        emit error(JsonParsingError, errorMsg);
    }
}
