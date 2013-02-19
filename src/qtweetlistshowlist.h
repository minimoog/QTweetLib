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

#ifndef QTWEETLISTSHOWLIST_H
#define QTWEETLISTSHOWLIST_H

#include "qtweetnetbase.h"

/**
 *   Show the specified list.
 *   Private lists will only be shown if the authenticated user owns the specified list.
 */
class QTWEETLIBSHARED_EXPORT QTweetListShowList : public QTweetNetBase
{
    Q_OBJECT
public:
    QTweetListShowList(QObject *parent = 0);
    QTweetListShowList(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void show(qint64 id, qint64 list);

signals:
    /** Emits the list */
    void parsedList(const QTweetList& list);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};

#endif // QTWEETLISTSHOWLIST_H
