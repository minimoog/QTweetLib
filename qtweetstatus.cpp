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

#include "qtweetstatus.h"
#include <QString>
#include <QSharedData>
#include "qtweetuser.h"

class QTweetStatusData : public QSharedData {
public:
    QTweetStatusData() : id(0), inReplyToStatusId(0), inReplyToUserId(0) {}

    QString createdAt;  // ### TODO: Use QDateTime
    qint64 id;
    QString text;
    QString source;
    bool truncated;     //
    qint64 inReplyToStatusId;
    qint64 inReplyToUserId;
    bool favorited;     //
    QString inReplyToScreenName;
    QTweetUser user;
};

QTweetStatus::QTweetStatus() : data(new QTweetStatusData)
{
}

QTweetStatus::QTweetStatus(const QTweetStatus &rhs) : data(rhs.data)
{
}

QTweetStatus &QTweetStatus::operator=(const QTweetStatus &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

QTweetStatus::~QTweetStatus()
{
}

void QTweetStatus::setId(qint64 id)
{
    data->id = id;
}

qint64 QTweetStatus::id() const
{
    return data->id;
}

void QTweetStatus::setText(const QString &text)
{
    data->text = text;
}

QString QTweetStatus::text() const
{
    return data->text;
}

void QTweetStatus::setSource(const QString &source)
{
    data->source = source;
}

QString QTweetStatus::source() const
{
    return data->source;
}

void QTweetStatus::setInReplyToStatusId(qint64 id)
{
    data->inReplyToStatusId = id;
}

qint64 QTweetStatus::inReplyToStatusId() const
{
    return data->inReplyToStatusId;
}

void QTweetStatus::setInReplyToUserId(qint64 id)
{
    data->inReplyToUserId = id;
}

qint64 QTweetStatus::inReplyToUserId() const
{
    return data->inReplyToUserId;
}

void QTweetStatus::setInReplyToScreenName(const QString &screenName)
{
    data->inReplyToScreenName = screenName;
}

QString QTweetStatus::inReplyToScreenName() const
{
    return data->inReplyToScreenName;
}

void QTweetStatus::setUser(const QTweetUser &user)
{
    data->user = user;
}

QTweetUser QTweetStatus::user() const
{
    return data->user;
}
