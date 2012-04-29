/* Copyright (c) 2011, Antonie Jovanoski
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

#ifndef QTWEETSTATUSRETWEETEDBY_H
#define QTWEETSTATUSRETWEETEDBY_H

#include "qtweetnetbase.h"

/**
 *  Show user objects of up to 100 members who retweeted the status.
 */
class QTWEETLIBSHARED_EXPORT QTweetStatusRetweetedBy : public QTweetNetBase
{
    Q_OBJECT
    Q_PROPERTY(qint64 tweetid READ tweetid WRITE setTweetid)
    Q_PROPERTY(int count READ count WRITE setCount)
    Q_PROPERTY(int page READ page WRITE setPage)

public:
    explicit QTweetStatusRetweetedBy(QObject *parent = 0);
    QTweetStatusRetweetedBy(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void fetch(qint64 tweetid, int count = 0, int page = 0);
    void get();
    
    void setTweetid(qint64 id) { m_tweetid = id; }
    qint64 tweetid() const { return m_tweetid; }

    void setCount(int count) { m_count = count; }
    int count() const { return m_count; }

    void setPage(int page) { m_page = page; }
    int page() const { return m_page; }

signals:
    /** Emits the users who retweeted the tweet */
    void parsedUsers(const QList<QTweetUser>& users);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
    
public slots:
    
private:
    qint64 m_tweetid;
    int m_count;
    int m_page;
};

#endif // QTWEETSTATUSRETWEETEDBY_H
