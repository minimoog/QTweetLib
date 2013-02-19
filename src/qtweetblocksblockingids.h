/* Copyright 2011 Antonie Jovanoski
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

#ifndef QTWEETBLOCKSBLOCKINGIDS_H
#define QTWEETBLOCKSBLOCKINGIDS_H

#include "qtweetnetbase.h"

/**
 *  Emits a list of numeric user ids the authenticating user is blocking.
 */
class QTWEETLIBSHARED_EXPORT QTweetBlocksBlockingIDs : public QTweetNetBase
{
    Q_OBJECT
public:
    explicit QTweetBlocksBlockingIDs(QObject *parent = 0);
    QTweetBlocksBlockingIDs(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void getIDs();

signals:
    void finishedGettingIDs(const QList<qint64> useridlist);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};

#endif // QTWEETBLOCKSBLOCKINGIDS_H
