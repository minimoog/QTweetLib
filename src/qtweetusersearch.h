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

#ifndef QTWEETUSERSEARCH_H
#define QTWEETUSERSEARCH_H

#include "qtweetnetbase.h"

/**
 *   Runs a search for users
 */
class QTWEETLIBSHARED_EXPORT QTweetUserSearch : public QTweetNetBase
{
    Q_OBJECT
public:
    QTweetUserSearch(QObject *parent = 0);
    QTweetUserSearch(OAuthTwitter* oauthTwitter, QObject *parent);
    void search(const QString& query,
                int perPage = 0,
                int page = 0,
                bool includeEntities = false);

signals:
    /** Emits list of users */
    void parsedUserInfoList(const QList<QTweetUser>& userInfoList);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};

#endif // QTWEETUSERSEARCH_H
