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

#ifndef QTWEETLISTADDMEMBER_H
#define QTWEETLISTADDMEMBER_H

#include "qtweetnetbase.h"

/**
 *   Add a member to a list.
 *   The authenticated user must own the list to be able to add members to it.
 *   Lists are limited to having 500 members.
 */
class QTWEETLIBSHARED_EXPORT QTweetListAddMember : public QTweetNetBase
{
    Q_OBJECT
public:
    QTweetListAddMember(QObject *parent = 0);
    QTweetListAddMember(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void add(qint64 user,
             qint64 list,
             qint64 memberid);

signals:
    /** Emits list in which user was added */
    void parsedList(const QTweetList& list);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};

#endif // QTWEETLISTADDMEMBER_H
