/* Copyright (c) 2012, Antonie Jovanoski
 *
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
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
