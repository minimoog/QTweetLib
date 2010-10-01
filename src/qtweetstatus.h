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

#ifndef QTWEETSTATUS_H
#define QTWEETSTATUS_H

#include <QVariant>
#include <QSharedDataPointer>
#include "qtweetlib_global.h"

class QDateTime;
class QTweetUser;
class QTweetStatusData;

// ### TODO: Places and coordinates

/*!
    Stores tweet info
 */
class QTWEETLIBSHARED_EXPORT QTweetStatus
{
public:
    QTweetStatus();
    QTweetStatus(const QTweetStatus &other);
    QTweetStatus &operator=(const QTweetStatus &other);
    ~QTweetStatus();

    void setId(qint64 id);
    qint64 id() const;
    void setText(const QString& text);
    QString text() const;
    void setCreatedAt(const QString& twitterDate);
    QDateTime createdAt() const;
    void setInReplyToUserId(qint64 id);
    qint64 inReplyToUserId() const;
    void setInReplyToScreenName(const QString& screenName);
    QString inReplyToScreenName() const;
    void setInReplyToStatusId(qint64 id);
    qint64 inReplyToStatusId() const;
    void setFavorited(bool fav);
    bool favorited() const;
    void setSource(const QString& source);
    QString source() const;
    void setUser(const QTweetUser& user);
    QTweetUser user() const;
    void setRetweetedStatus(const QTweetStatus& status);
    QTweetStatus retweetedStatus() const;

private:
    QSharedDataPointer<QTweetStatusData> d;
};

Q_DECLARE_METATYPE(QTweetStatus)

#endif // QTWEETSTATUS_H
