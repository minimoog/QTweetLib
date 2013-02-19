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

#ifndef QTWEETFAVORITESDESTROY_H
#define QTWEETFAVORITESDESTROY_H

#include "qtweetnetbase.h"

/**
 *   Un-favorites the status specified in the ID parameter as the authenticating user.
 *   Emits the un-favorited status when successful.
 */
class QTWEETLIBSHARED_EXPORT QTweetFavoritesDestroy : public QTweetNetBase
{
    Q_OBJECT
public:
    QTweetFavoritesDestroy(QObject *parent = 0);
    QTweetFavoritesDestroy(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void unfavorite(qint64 statusid, bool includeEntities = false);

signals:
    /** Emits the unfavorited status */
    void parsedStatus(const QTweetStatus& status);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};

#endif // QTWEETFAVORITESDESTROY_H
