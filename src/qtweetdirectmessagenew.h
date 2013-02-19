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

#ifndef QTWEETDIRECTMESSAGENEW_H
#define QTWEETDIRECTMESSAGENEW_H

#include "qtweetnetbase.h"

/**
 *   Sends a new direct message to the specified user from the authenticating user.
 */
class QTWEETLIBSHARED_EXPORT QTweetDirectMessageNew : public QTweetNetBase
{
    Q_OBJECT
public:
    QTweetDirectMessageNew(QObject *parent = 0);
    QTweetDirectMessageNew(OAuthTwitter *oauhtTwitter, QObject *parent = 0);
    void post(qint64 user,
              const QString& text,
              bool includeEntities = false);
    void post(const QString& screenName,
              const QString& text,
              bool includeEntities = false);

signals:
    /** Emits direct message who was sent */
    void parsedDirectMessage(const QTweetDMStatus& message);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};

#endif // QTWEETDIRECTMESSAGENEW_H
