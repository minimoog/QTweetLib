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

#ifndef QTWEETBLOCKSBLOCKING_H
#define QTWEETBLOCKSBLOCKING_H

#include "qtweetnetbase.h"

/**
 *  Emits the list of users that the authenticating user is blocking.
 */
class QTWEETLIBSHARED_EXPORT QTweetBlocksBlocking : public QTweetNetBase
{
    Q_OBJECT
public:
    explicit QTweetBlocksBlocking(QObject *parent = 0);
    QTweetBlocksBlocking(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void getBlocks(int page = 0, bool includeEntities = false);

signals:
    void finishedGettingBlocks(const QList<QTweetUser>& users);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};

#endif // QTWEETBLOCKSBLOCKING_H
