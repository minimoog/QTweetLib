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

#ifndef QTWEETFAVORITES_H
#define QTWEETFAVORITES_H

#include "qtweetnetbase.h"

/**
 *   Fetches favorite statuses for the authenticating user or
 *   user specified by the ID parameter
 */
class QTWEETLIBSHARED_EXPORT QTweetFavorites : public QTweetNetBase
{
    Q_OBJECT
public:
    QTweetFavorites(QObject *parent = 0);
    QTweetFavorites(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void fetch(qint64 id = 0,
               int page = 0,
               bool includeEntities = false);

signals:
    /** Emits list of favorited statuses */
    void parsedFavorites(const QList<QTweetStatus>& favorites);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};

#endif // QTWEETFAVORITES_H
