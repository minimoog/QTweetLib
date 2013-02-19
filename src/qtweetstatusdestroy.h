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

#ifndef QTWEETSTATUSDESTROY_H
#define QTWEETSTATUSDESTROY_H

#include "qtweetnetbase.h"

/**
 *   Class for destroying specified tweet (user must be author of tweet)
 */
class QTWEETLIBSHARED_EXPORT QTweetStatusDestroy : public QTweetNetBase
{
    Q_OBJECT
public:
    QTweetStatusDestroy(QObject *parent = 0);
    QTweetStatusDestroy(OAuthTwitter* oauthTwitter, QObject *parent = 0);
    void destroy(qint64 id,
                 bool trimUser = false);

signals:
    /** Emits deleted status */
    void deletedStatus(const QTweetStatus& status);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};

#endif // QTWEETSTATUSDESTROY_H
