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

#ifndef QTWEETUSERSHOW_H
#define QTWEETUSERSHOW_H

#include "qtweetnetbase.h"

/**
 *   Class for fetching extended information for given user
 */
class QTWEETLIBSHARED_EXPORT QTweetUserShow : public QTweetNetBase
{
    Q_OBJECT
public:
    QTweetUserShow(QObject *parent = 0);
    QTweetUserShow(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void fetch(qint64 userid = 0,
               bool includeEntities = false);
    void fetch(const QString& screenName,
               bool includeEntities = false);

signals:
    /** Emits user */
    void parsedUserInfo(const QTweetUser& userInfo);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};

#endif // QTWEETUSERSHOW_H
