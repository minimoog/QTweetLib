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

#ifndef QTWEETSTATUSSHOW_H
#define QTWEETSTATUSSHOW_H

#include "qtweetnetbase.h"

/**
 *   Class for fetching single tweet
 */
class QTWEETLIBSHARED_EXPORT QTweetStatusShow : public QTweetNetBase
{
    Q_OBJECT
    Q_PROPERTY(qint64 tweetid READ tweetid WRITE setTweetid)
    Q_PROPERTY(bool trimUser READ isTrimUser WRITE setTrimUser)
    Q_PROPERTY(bool includeMyRetweet READ includeMyRetweet WRITE setIncludeMyRetweet)
    Q_PROPERTY(bool includeEntities READ includeEntities WRITE setIncludeEntities)

public:
    QTweetStatusShow(QObject *parent = 0);
    QTweetStatusShow(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void fetch(qint64 id,
               bool trimUser = false,
               bool includeMyRetweet = false,
               bool includeMyRetweet = false);
    void get();

    qint64 tweetid() const { return m_tweetid; }
    void setTweetid(qint64 id) { m_tweetid = id; }

    void setTrimUser(bool trimUser) { m_trimUser = trimUser; }
    bool isTrimUser() const { return m_trimUser; }

    void setIncludeMyRetweet(bool includeMyRetweet) { m_includeMyRetweet = includeMyRetweet; }
    bool includeMyRetweet() const { return m_includeMyRetweet; }

    void setIncludeEntities(bool includeEntities) { m_includeEntities = includeEntities; }
    bool includeEntities() const { return m_includeEntities; }

signals:
    /** Emits specified tweet */
    void parsedStatus(const QTweetStatus& status);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);

private:
    // ### TODO: Use pimpl
    qint64 m_tweetid;
    bool m_trimUser;
    bool m_includeMyRetweet;
    bool m_includeEntities;
};

#endif // QTWEETSTATUSSHOW_H
