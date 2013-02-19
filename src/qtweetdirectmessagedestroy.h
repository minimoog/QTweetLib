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

#ifndef QTWEETDIRECTMESSAGEDESTROY_H
#define QTWEETDIRECTMESSAGEDESTROY_H

#include "qtweetnetbase.h"

/**
 *   Destroys the direct message.
 *   The authenticating user must be the recipient of the specified direct message.
 */
class QTWEETLIBSHARED_EXPORT QTweetDirectMessageDestroy : public QTweetNetBase
{
    Q_OBJECT
public:
    QTweetDirectMessageDestroy(QObject *parent = 0);
    QTweetDirectMessageDestroy(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void destroyMessage(qint64 id, bool includeEntities = false);

signals:
    /** emits destroyed direct message */
    void parsedDirectMessage(const QTweetDMStatus& message);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};

#endif // QTWEETDIRECTMESSAGEDESTROY_H
