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

#ifndef QTWEETSTATUSRETWEETBYID_H
#define QTWEETSTATUSRETWEETBYID_H

#include "qtweetnetbase.h"

/**
 *  Show user ids of up to 100 users who retweeted the status.
 */
class QTWEETLIBSHARED_EXPORT QTweetStatusRetweetByID : public QTweetNetBase
{
    Q_OBJECT
    Q_PROPERTY(qint64 tweetid READ tweetid WRITE setTweetid)
    Q_PROPERTY(int count READ count WRITE setCount)
    Q_PROPERTY(int page READ page WRITE setPage)

public:
    explicit QTweetStatusRetweetByID(QObject *parent = 0);
    QTweetStatusRetweetByID(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void fetch(qint64 tweetid, int count = 0, int page = 0);
    void get();

    void setTweetid(qint64 id) { m_tweetid = id; }
    qint64 tweetid() const { return m_tweetid; }

    void setCount(int count) { m_count = count; }
    int count() const { return m_count; }

    void setPage(int page) { m_page = page; }
    int page() const { return m_page; }
    
signals:
    /** Emits the users id who retweeted the tweet */
    void parsedUsersID(const QList<qint64>& listid);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
    
private:
    qint64 m_tweetid;
    int m_count;
    int m_page;
};

#endif // QTWEETSTATUSRETWEETBYID_H
