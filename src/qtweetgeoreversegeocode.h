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

#ifndef QTWEETGEOREVERSEGEOCODE_H
#define QTWEETGEOREVERSEGEOCODE_H

#include "qtweetnetbase.h"
#include "qtweetplace.h"

/**
 *  Given a latitude and a longitude, searches up to 20 places that can be used
 *  as placeId when updating a status
 */
class QTWEETLIBSHARED_EXPORT QTweetGeoReverseGeoCode : public QTweetNetBase
{
    Q_OBJECT
public:
    QTweetGeoReverseGeoCode(QObject *parent = 0);
    QTweetGeoReverseGeoCode(OAuthTwitter *oauthTwitter, QObject *parent = 0);

    void getPlaces(const QTweetGeoCoord& latLong,
                   int accuracy = 0,
                   QTweetPlace::Type granularity = QTweetPlace::Neighborhood,
                   int maxResults = 0);

signals:
    /** Emits list of places */
    void parsedPlaces(const QList<QTweetPlace>& places);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};

#endif // QTWEETGEOREVERSEGEOCODE_H
