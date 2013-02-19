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

#ifndef QTWEETSEARCH_H
#define QTWEETSEARCH_H

#include "qtweetnetbase.h"

/**
 *  Gets tweets that match a specified query
 */
class QTWEETLIBSHARED_EXPORT QTweetSearch : public QTweetNetBase
{
    Q_OBJECT
public:
    QTweetSearch(QObject *parent = 0);
    QTweetSearch(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void start(const QString& query,
               const QString& lang = QString(),
               /* const QString& locale = QString(), */
               int rpp = 0,
               int page = 0,
               qint64 sinceid = 0
               //geocode ### TODO
               //resultType ### TODO
               );
    void startWithCustomQuery(const QByteArray& encodedQuery);

signals:
    /** Emits page of search results */
    void parsedPageResults(const QTweetSearchPageResults& pageResults);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};

#endif // QTWEETSEARCH_H
