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

#ifndef QTWEETBLOCKSEXISTS_H
#define QTWEETBLOCKSEXISTS_H

#include "qtweetnetbase.h"

/**
 *  Emits finishedIsBlocked if the authenticating user is blocking a target user.
 *  Will return the blocked user's object if a block exists,
 *  and error with a HTTP 404 response code otherwise.
 */
class QTweetBlocksExists : public QTweetNetBase
{
    Q_OBJECT
public:
    explicit QTweetBlocksExists(QObject *parent = 0);
    QTweetBlocksExists(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void isBlocked(qint64 userid, bool includeEntities = false);
    void isBlocked(const QString& screenName, bool includeEntities = false);

signals:
    void finishedIsBlocked(const QTweetUser& user);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};

#endif // QTWEETBLOCKSEXISTS_H
