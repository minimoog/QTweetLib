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

#ifndef QTWEETFRIENDSHIPCREATE_H
#define QTWEETFRIENDSHIPCREATE_H

#include "qtweetnetbase.h"

/**
 *   Allows the authenticating users to follow the user specified in the ID parameter.
 */
class QTWEETLIBSHARED_EXPORT QTweetFriendshipCreate : public QTweetNetBase
{
    Q_OBJECT
public:
    QTweetFriendshipCreate(QObject *parent = 0);
    QTweetFriendshipCreate(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void create(qint64 userid,
                bool follow = false,
                bool includeEntities = false);
    void create(const QString& screenName,
                bool follow = false,
                bool includeEntities = false);

signals:
    /** Emits the befriended user */
    void parsedUser(const QTweetUser& user);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};

#endif // QTWEETFRIENDSHIPCREATE_H
