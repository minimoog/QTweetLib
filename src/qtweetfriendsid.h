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

#ifndef QTWEETFRIENDSID_H
#define QTWEETFRIENDSID_H

#include "qtweetnetbase.h"

/**
 *   Returns an list of numeric IDs for every user the specified user is following.
 */
class QTWEETLIBSHARED_EXPORT QTweetFriendsID : public QTweetNetBase
{
    Q_OBJECT
public:
    QTweetFriendsID(QObject *parent = 0);
    QTweetFriendsID(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void fetch(qint64 user,
               const QString& cursor = QString("-1"));
    void fetch(const QString& screenName,
               const QString& cursor = QString("-1"));

signals:
    /** Emits one page of ids
     * @param ids list of ids user is following
     * @param nextCursor cursor for next page, "0" if there is no page
     * @param prevCursor cursor for prev page "0" if there is no page
     */
    void parsedIDs(const QList<qint64>& ids,
                   const QString& nextCursor,
                   const QString& prevCursor);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};

#endif // QTWEETFRIENDSID_H
