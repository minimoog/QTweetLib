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

#ifndef QTWEETGEOSEARCH_H
#define QTWEETGEOSEARCH_H

#include "qtweetplace.h"
#include "qtweetnetbase.h"


/**
 *  Search for places that can be attached to a statuses/update
 *  @see http://dev.twitter.com/doc/get/geo/search
 */
class QTWEETLIBSHARED_EXPORT QTweetGeoSearch : public QTweetNetBase
{
    Q_OBJECT
public:
    QTweetGeoSearch(QObject *parent = 0);
    QTweetGeoSearch(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void search(const QTweetGeoCoord& latLong = QTweetGeoCoord(),
                const QString& query = QString(),
                const QString& ip = QString(),
                QTweetPlace::Type granularity = QTweetPlace::Neighborhood,
                int accuracy = 0,
                int maxResults = 0,
                const QString& containedWithin = QString());
                // ### TODO Atributes, not enough documentation

signals:
    /** Emits list of places */
    void parsedPlaces(const QList<QTweetPlace>& places);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};

#endif // QTWEETGEOSEARCH_H
