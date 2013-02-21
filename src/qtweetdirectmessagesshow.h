/* Copyright 2013 Antonie Jovanoski
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

#ifndef QTWEETDIRECTMESSAGESSHOW_H
#define QTWEETDIRECTMESSAGESSHOW_H

#include "qtweetnetbase.h"

class QTweetDMStatus;

/**
 * Class for fetching a single direct message
 */
class QTWEETLIBSHARED_EXPORT QTweetDirectMessagesShow : public QTweetNetBase
{
    Q_OBJECT

public:
    explicit QTweetDirectMessagesShow(QObject *parent = 0);
    QTweetDirectMessagesShow(OAuthTwitter* oauthTwitter, QObject *parent = 0);
    void fetch(qint64 id);
    
signals:
    /** Emits the fetched direct message */
    void parsedDirectMessage(const QTweetDMStatus& message);

protected slots:
    void parseJsonFinished(const QJsonDocument& jsonDocument);
};

#endif // QTWEETDIRECTMESSAGESSHOW_H
