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

#ifndef QTWEETLISTSUBSCRIBE_H
#define QTWEETLISTSUBSCRIBE_H

#include "qtweetnetbase.h"

/**
 *   Make the authenticated user follow the specified list.
 */
class QTWEETLIBSHARED_EXPORT QTweetListSubscribe : public QTweetNetBase
{
    Q_OBJECT
public:
    QTweetListSubscribe(QObject *parent = 0);
    QTweetListSubscribe(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void follow(qint64 user,
                qint64 list);

signals:
    /** Emits the specified list */
    void parsedList(const QTweetList& list);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};

#endif // QTWEETLISTSUBSCRIBE_H
