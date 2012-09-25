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
#include "qtweetgeoplacecreate.h"
#include "qtweetplace.h"
#include "qtweetconvert.h"
#include <QJsonDocument>
#include <QJsonObject>

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
    QUrl urlPost(url);
    QUrlQuery urlQuery;

    urlQuery.addQueryItem("name", name);
    urlQuery.addQueryItem("contained_within", containedWithin);
    urlQuery.addQueryItem("token", token);
    urlQuery.addQueryItem("lat", QString::number(latLong.latitude()));
    urlQuery.addQueryItem("long", QString::number(latLong.longitude()));

    urlPost.setQuery(urlQuery);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(urlPost, OAuth::POST);

    QNetworkRequest req(url);
    req.setRawHeader(AUTH_HEADER, oauthHeader);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QByteArray statusPost = urlPost.toEncoded(QUrl::RemoveScheme | QUrl::RemoveAuthority | QUrl::RemovePath);
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
