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
    Q_PROPERTY(qint64 id READ id WRITE setID)
    Q_PROPERTY(bool trimUser READ trimUser WRITE setTrimUser)
    Q_PROPERTY(bool includeEntities READ includeEntities WRITE setIncludeEntities)
    Q_PROPERTY(bool includeMyRetweet READ includeMyRetweet WRITE setIncludeMyRetweet)

public:
    QTweetStatusShow(QObject *parent = 0);
    QTweetStatusShow(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void fetch(qint64 id,
               bool trimUser = false,
               bool includeEntities = false,
               bool includeMyRetweet = false);
    void get();

    void setID(qint64 id) { m_id = id; }
    qint64 id() const { return m_id; }

    void setTrimUser(bool trimUser) { m_trimUser = trimUser; }
    bool trimUser() const { return m_trimUser; }

    void setIncludeEntities(bool includeEntities) { m_includeEntities = includeEntities; }
    bool includeEntities() const { return m_includeEntities; }

    void setIncludeMyRetweet(bool includeMyRetweet) { m_includeMyRetweet = includeMyRetweet; }
    bool includeMyRetweet() const { return m_includeMyRetweet; }

signals:
    /** Emits specified tweet */
    void parsedStatus(const QTweetStatus& status);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);

private:
    qint64 m_id;
    bool m_trimUser;
    bool m_includeEntities;
    bool m_includeMyRetweet;
};

#endif // QTWEETSTATUSSHOW_H
