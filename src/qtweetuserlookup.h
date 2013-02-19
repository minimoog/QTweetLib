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

#ifndef QTWEETUSERLOOKUP_H
#define QTWEETUSERLOOKUP_H

#include <QStringList>
#include "qtweetnetbase.h"

class QTweetUser;

/**
 *   Class for fetching up to 100 users and theirs most recent status
 */
class QTWEETLIBSHARED_EXPORT QTweetUserLookup : public QTweetNetBase
{
    Q_OBJECT
public:
    QTweetUserLookup(QObject *parent = 0);
    QTweetUserLookup(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void fetch(const QList<qint64>& useridList = QList<qint64>(),
               const QStringList& screenNameList = QStringList());

signals:
    /** Emits list of users */
    void parsedUserInfoList(const QList<QTweetUser>& userInfoList);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};

#endif // QTWEETUSERLOOKUP_H
