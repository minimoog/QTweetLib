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

#ifndef QTWEETSTATUSUPDATE_H
#define QTWEETSTATUSUPDATE_H

#include "qtweetnetbase.h"
#include "qtweetgeocoord.h"

/**
 *   Class for updating user status (posting tweet)
 */
class QTWEETLIBSHARED_EXPORT QTweetStatusUpdate : public QTweetNetBase
{
    Q_OBJECT
public:
    QTweetStatusUpdate(QObject *parent = 0);
    QTweetStatusUpdate(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void post(const QString& status,
              qint64 inReplyToStatus = 0,
              const QTweetGeoCoord& latLong = QTweetGeoCoord(),
              const QString& placeid = QString(),
              bool displayCoordinates = false,
              bool trimUser = false,
              bool includeEntities = false);

signals:
    /** Emits posted status */
    void postedStatus(const QTweetStatus& status);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};

#endif // QTWEETSTATUSUPDATE_H
