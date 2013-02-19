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

#ifndef QTWEETBLOCKSDESTROY_H
#define QTWEETBLOCKSDESTROY_H

#include "qtweetnetbase.h"

/**
 *  Un-blocks the user specified in the ID parameter for the authenticating user.
 *  Returns the un-blocked user when successful.
 */
class QTWEETLIBSHARED_EXPORT QTweetBlocksDestroy : public QTweetNetBase
{
    Q_OBJECT
public:
    explicit QTweetBlocksDestroy(QObject *parent = 0);
    QTweetBlocksDestroy(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void unblock(qint64 userid, bool includeEntities = false);
    void unblock(const QString& screenName, bool includeEntities = false);

signals:
    /** Emits the ublocked user */
    void finishedUnblocking(const QTweetUser& user);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};

#endif // QTWEETBLOCKSDESTROY_H
