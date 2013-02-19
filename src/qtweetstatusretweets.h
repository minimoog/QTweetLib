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

#ifndef QTWEETSTATUSRETWEETS_H
#define QTWEETSTATUSRETWEETS_H

#include "qtweetnetbase.h"

/**
 *   Class to fetch up to 100 of the first retweets of a given tweet.
 */
class QTWEETLIBSHARED_EXPORT QTweetStatusRetweets : public QTweetNetBase
{
    Q_OBJECT
    Q_PROPERTY(qint64 tweetid READ tweetid WRITE setTweetid)
    Q_PROPERTY(int count READ count WRITE setCount)
    Q_PROPERTY(bool trimUser READ isTrimUser WRITE setTrimUser)

public:
    explicit QTweetStatusRetweets(QObject *parent = 0);
    QTweetStatusRetweets(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void fetch(qint64 id, int count = 0, bool trimUser = false);
    void get();

    void setTweetid(qint64 id) { m_tweetid = id; }
    qint64 tweetid() const { return m_tweetid; }

    void setCount(int count) { m_count = count; }
    int count() const { return m_count; }

    void setTrimUser(bool trimUser) { m_trimUser = trimUser; }
    bool isTrimUser() const { return m_trimUser; }

signals:
    /** Emits retweet status list */
    void parsedStatuses(const QList<QTweetStatus>& statuses);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);

private:
    // ### TODO: Use pimpl
    qint64 m_tweetid;
    int m_count;
    bool m_trimUser;
};

#endif // QTWEETSTATUSRETWEETS_H
