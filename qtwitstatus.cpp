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

#include "qtwitstatus.h"
#include <QString>
#include <QSharedData>
#include "qtwituser.h"

class QTwitStatusData : public QSharedData {
public:
    QTwitStatusData() : id(0), inReplyToStatusId(0), inReplyToUserId(0) {}

    QString createdAt;  // ### TODO: Use QDateTime
    qint64 id;
    QString text;
    QString source;
    bool truncated;     //
    qint64 inReplyToStatusId;
    qint64 inReplyToUserId;
    bool favorited;     //
    QString inReplyToScreenName;
    QTwitUser user;
};

QTwitStatus::QTwitStatus() : data(new QTwitStatusData)
{
}

QTwitStatus::QTwitStatus(const QTwitStatus &rhs) : data(rhs.data)
{
}

QTwitStatus &QTwitStatus::operator=(const QTwitStatus &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

QTwitStatus::~QTwitStatus()
{
}

void QTwitStatus::setId(qint64 id)
{
    data->id = id;
}

qint64 QTwitStatus::id() const
{
    return data->id;
}

void QTwitStatus::setText(const QString &text)
{
    data->text = text;
}

QString QTwitStatus::text() const
{
    return data->text;
}

void QTwitStatus::setSource(const QString &source)
{
    data->source = source;
}

QString QTwitStatus::source() const
{
    return data->source;
}

void QTwitStatus::setInReplyToStatusId(qint64 id)
{
    data->inReplyToStatusId = id;
}

qint64 QTwitStatus::inReplyToStatusId() const
{
    return data->inReplyToStatusId;
}

void QTwitStatus::setInReplyToUserId(qint64 id)
{
    data->inReplyToUserId = id;
}

qint64 QTwitStatus::inReplyToUserId() const
{
    return data->inReplyToUserId;
}

void QTwitStatus::setInReplyToScreenName(const QString &screenName)
{
    data->inReplyToScreenName = screenName;
}

QString QTwitStatus::inReplyToScreenName() const
{
    return data->inReplyToScreenName;
}

void QTwitStatus::setUser(const QTwitUser &user)
{
    data->user = user;
}

QTwitUser QTwitStatus::user() const
{
    return data->user;
}
