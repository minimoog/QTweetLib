/* Copyright (c) 2010, Antonie Jovanoski
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
