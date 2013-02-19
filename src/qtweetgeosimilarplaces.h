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

#ifndef QTWEETGEOSIMILARPLACES_H
#define QTWEETGEOSIMILARPLACES_H

#include "qtweetnetbase.h"

class QTweetPlace;
class QTweetGeoCoord;

/**
 *  Locates places near the given coordinates which are similar in name
 *  @see http://dev.twitter.com/doc/get/geo/similar_places
 */
class QTWEETLIBSHARED_EXPORT QTweetGeoSimilarPlaces : public QTweetNetBase
{
    Q_OBJECT
public:
    QTweetGeoSimilarPlaces(QObject *parent = 0);
    QTweetGeoSimilarPlaces(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void get(const QTweetGeoCoord& latLong,
             const QString& name,
             const QString& containedWithin = QString());
    // ### TODO: Atributes, lack of documentation

signals:
    /** Emits places
      * @param places list of places
      * @param token token to create a new place
      */
    void parsedPlaces(const QList<QTweetPlace>& places, const QString& token);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};

#endif // QTWEETGEOSIMILARPLACES_H
