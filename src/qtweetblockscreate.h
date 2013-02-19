/* Copyright 2011 Antonie Jovanoski
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

#ifndef QTWEETBLOCKSCREATE_H
#define QTWEETBLOCKSCREATE_H

#include "qtweetnetbase.h"

/**
 *  Blocks the user specified in the ID parameter as the authenticating user.
 *  Destroys a friendship to the blocked user if it exists.
 *  Returns the blocked user when successful.
 */
class QTWEETLIBSHARED_EXPORT QTweetBlocksCreate : public QTweetNetBase
{
    Q_OBJECT
public:
    explicit QTweetBlocksCreate(QObject *parent = 0);
    QTweetBlocksCreate(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void create(qint64 userid, bool includeEnttities = false);
    void create(const QString& screenName, bool includeEntities = false);

signals:
    /** Emits the blocked user */
    void finishedCreatingBlock(const QTweetUser& user);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};

#endif // QTWEETBLOCKSCREATE_H
