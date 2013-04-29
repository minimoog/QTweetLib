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

#ifndef QTWEETRETWEETSOFME_H
#define QTWEETRETWEETSOFME_H

#include "qtweetnetbase.h"

/**
 * Returns the most recent tweets authored by the authenticating user that have been retweeted by others.
 */
class QTWEETLIBSHARED_EXPORT QTweetRetweetsOfMe : public QTweetNetBase
{
    Q_OBJECT
    Q_PROPERTY(int count READ count WRITE setCount)
    Q_PROPERTY(qint64 sinceId READ sinceId WRITE setSinceId)
    Q_PROPERTY(qint64 maxId READ maxId WRITE setMaxId)
    Q_PROPERTY(bool trimUser READ isTrimUser WRITE setTrimUser)
    Q_PROPERTY(bool includeEntities READ isIncludeEntities WRITE setIncludeEntities)
    Q_PROPERTY(bool includeUserEntities READ isIncludeUserEntities WRITE setIncludeUserEntities)

public:
    explicit QTweetRetweetsOfMe(QObject *parent = 0);
    QTweetRetweetsOfMe(OAuthTwitter *oauthTwitter, QObject *parent = 0);

    void setCount(int count) { m_count = count; }
    int count() const { return m_count; }

    void setSinceId(qint64 sinceId) { m_sinceId = sinceId; }
    qint64 sinceId() const { return m_sinceId; }

    void setMaxId(qint64 maxId) { m_maxId = maxId; }
    qint64 maxId() const { return m_maxId; }

    void setTrimUser(bool trimUser) { m_trimUser = trimUser; }
    bool isTrimUser() const { return m_trimUser; }

    void setIncludeEntities(bool includeEntities) { m_includeEntities = includeEntities; }
    bool isIncludeEntities() const { return m_includeEntities; }

    void setIncludeUserEntities(bool includeUserEntities) { m_includeUserEntities = includeUserEntities; }
    bool isIncludeUserEntities() const { return m_includeUserEntities; }

    void fetch(qint64 sinceId,
               qint64 maxId,
               int count,
               bool trimUser,
               bool includeEntities,
               bool includeUserEntities);
    void get();

signals:
    void parsedStatuses(const QList<QTweetStatus>& statuses);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);

private:
    qint64 m_sinceId;
    qint64 m_maxId;
    int m_count;
    bool m_trimUser;
    bool m_includeEntities;
    bool m_includeUserEntities;
};

#endif // QTWEETRETWEETSOFME_H
