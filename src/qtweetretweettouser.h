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

#ifndef QTWEETRETWEETTOUSER_H
#define QTWEETRETWEETTOUSER_H

#include "qtweetnetbase.h"

/**
 * Returns the recent retweets posted by users the specified user follows.
 */
class QTWEETLIBSHARED_EXPORT QTweetRetweetToUser : public QTweetNetBase
{
    Q_OBJECT

    Q_PROPERTY(qint64 userID READ userID WRITE setUserID)
    Q_PROPERTY(QString screenName READ screenName WRITE setScreenName)
    Q_PROPERTY(qint64 sinceID READ sinceID WRITE setSinceID)
    Q_PROPERTY(qint64 maxID READ maxID WRITE setMaxID)
    Q_PROPERTY(int count READ count WRITE setCount)
    Q_PROPERTY(int page READ page WRITE setPage)
    Q_PROPERTY(bool trimUser READ isTrimUser WRITE setTrimUser)
    Q_PROPERTY(bool includeEntities READ isIncludeEntities WRITE setIncludeEntities)

public:
    explicit QTweetRetweetToUser(QObject *parent = 0);
    QTweetRetweetToUser(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void fetch(qint64 userid,
               qint64 sinceid = 0,
               qint64 maxid = 0,
               int count = 0,
               int page = 0,
               bool trimUser = false,
               bool includeEntities = false);
   void fetch(const QString& screenName,
              qint64 sinceid = 0,
              qint64 maxid = 0,
              int count = 0,
              int page = 0,
              bool trimUser = false,
              bool includeEntities = false);
   void get();

   void setUserID(qint64 userid) { m_userid = userid; }
   qint64 userID() const { return m_userid; }

   void setScreenName(const QString& screenName) { m_screenName = screenName; }
   QString screenName() const { return m_screenName; }

   void setSinceID(qint64 sinceid) { m_sinceid = sinceid; }
   qint64 sinceID() const { return m_sinceid; }

   void setMaxID(qint64 maxid) { m_maxid = maxid; }
   qint64 maxID() const { return m_maxid; }

   void setCount(int count) { m_count = count; }
   int count() const { return m_count; }

   void setPage(int page) { m_page = page; }
   int page() const { return m_page; }

   void setTrimUser(bool trimUser) { m_trimUser = trimUser; }
   bool isTrimUser() const { return m_trimUser; }

   void setIncludeEntities(bool includeEntities) { m_includeEntities = includeEntities; }
   bool isIncludeEntities() const { return m_includeEntities; }

signals:
   /** Emits retweets status list */
   void parsedStatuses(const QList<QTweetStatus>& statuses);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);

private:
    qint64 m_userid;
    QString m_screenName;
    qint64 m_sinceid;
    qint64 m_maxid;
    int m_count;
    int m_page;
    bool m_trimUser;
    bool m_includeEntities;
};

#endif // QTWEETRETWEETTOUSER_H
