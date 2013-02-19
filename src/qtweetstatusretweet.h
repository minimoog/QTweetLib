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

#ifndef QTWEETSTATUSRETWEET_H
#define QTWEETSTATUSRETWEET_H

#include "qtweetnetbase.h"

/**
 *   Class for retweeting tweet
 */
class QTWEETLIBSHARED_EXPORT QTweetStatusRetweet : public QTweetNetBase
{
    Q_OBJECT
    Q_PROPERTY(qint64 id READ id WRITE setID)
    Q_PROPERTY(bool trimUser READ trimUser WRITE setTrimUser)
    Q_PROPERTY(bool includeEntities READ includeEntities WRITE setIncludeEntities)

public:
    QTweetStatusRetweet(QObject *parent = 0);
    QTweetStatusRetweet(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void retweet(qint64 id,
                 bool trimUser = false,
                 bool includeEntities = false);
    void retweet();

    void setID(qint64 id) { m_id = id; }
    qint64 id() const { return m_id; }

    void setTrimUser(bool trimUser) { m_trimUser = trimUser; }
    bool trimUser() const { return m_trimUser; }

    void setIncludeEntities(bool includeEntities) { m_includeEntities = includeEntities; }
    bool includeEntities() const { return m_includeEntities; }

signals:
    /** Emits retweeted tweet */
    void postedRetweet(const QTweetStatus& status);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);

private:
    qint64 m_id;
    bool m_trimUser;
    bool m_includeEntities;
};

#endif // QTWEETSTATUSRETWEET_H
