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

#ifndef QTWEETDIRECTMESSAGES_H
#define QTWEETDIRECTMESSAGES_H

#include "qtweetnetbase.h"

class QTweetDMStatus;

/**
 *   Class for fetching direct messages
 */
class QTWEETLIBSHARED_EXPORT QTweetDirectMessages : public QTweetNetBase
{
    Q_OBJECT
public:
    QTweetDirectMessages(QObject *parent = 0);
    QTweetDirectMessages(OAuthTwitter* oauthTwitter, QObject *parent = 0);
    void fetch(qint64 sinceid = 0,
               qint64 maxid = 0,
               int count = 0,
               bool includeEntities = false);

signals:
    /** Emits parsed direct messages list */
    void parsedDirectMessages(const QList<QTweetDMStatus>& messages);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};

#endif // QTWEETDIRECTMESSAGES_H
