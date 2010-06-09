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

#include "qtwitterstatus.h"
#include <QString>
#include <QSharedData>
#include "qtwitteruser.h"

class QtwitterStatusData : public QSharedData {
public:
    QtwitterStatusData() : id(0), inReplyToStatusId(0), inReplyToUserId(0) {}

    QString createdAt;  // ### TODO: Use QDateTime
    qint64 id;
    QString text;
    QString source;
    bool truncated;     //
    qint64 inReplyToStatusId;
    qint64 inReplyToUserId;
    bool favorited;     //
    QString inReplyToScreenName;
    QtwitterUser user;
};

QtwitterStatus::QtwitterStatus() : data(new QtwitterStatusData)
{
}

QtwitterStatus::QtwitterStatus(const QtwitterStatus &rhs) : data(rhs.data)
{
}

QtwitterStatus &QtwitterStatus::operator=(const QtwitterStatus &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

QtwitterStatus::~QtwitterStatus()
{
}

void QtwitterStatus::setId(qint64 id)
{
    data->id = id;
}

qint64 QtwitterStatus::id() const
{
    return data->id;
}

void QtwitterStatus::setText(const QString &text)
{
    data->text = text;
}

QString QtwitterStatus::text() const
{
    return data->text;
}

void QtwitterStatus::setSource(const QString &source)
{
    data->source = source;
}

QString QtwitterStatus::source() const
{
    return data->source;
}

void QtwitterStatus::setInReplyToStatusId(qint64 id)
{
    data->inReplyToStatusId = id;
}

qint64 QtwitterStatus::inReplyToStatusId() const
{
    return data->inReplyToStatusId;
}

void QtwitterStatus::setInReplyToUserId(qint64 id)
{
    data->inReplyToUserId = id;
}

qint64 QtwitterStatus::inReplyToUserId() const
{
    return data->inReplyToUserId;
}

void QtwitterStatus::setInReplyToScreenName(const QString &screenName)
{
    data->inReplyToScreenName = screenName;
}

QString QtwitterStatus::inReplyToScreenName() const
{
    return data->inReplyToScreenName;
}

void QtwitterStatus::setUser(const QtwitterUser &user)
{
    data->user = user;
}

QtwitterUser QtwitterStatus::user() const
{
    return data->user;
}
