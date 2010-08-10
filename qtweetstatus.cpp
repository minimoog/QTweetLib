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
#include "qtweetuser.h"

QTweetStatus::QTweetStatus()
{
}

void QTweetStatus::setId(qint64 id)
{
    m_statusInfo.insert(QTweetStatus::Id, id);
}

qint64 QTweetStatus::id() const
{
    return m_statusInfo.value(QTweetStatus::Id).toLongLong();
}

void QTweetStatus::setText(const QString &text)
{
    m_statusInfo.insert(QTweetStatus::Text, text);
}

QString QTweetStatus::text() const
{
    return m_statusInfo.value(QTweetStatus::Text).toString();
}

void QTweetStatus::setSource(const QString &source)
{
    m_statusInfo.insert(QTweetStatus::Source, source);
}

QString QTweetStatus::source() const
{
    return m_statusInfo.value(QTweetStatus::Source).toString();
}

void QTweetStatus::setInReplyToStatusId(qint64 id)
{
    m_statusInfo.insert(QTweetStatus::InReplyToStatusId, id);
}

qint64 QTweetStatus::inReplyToStatusId() const
{
    return m_statusInfo.value(QTweetStatus::InReplyToStatusId).toLongLong();
}

void QTweetStatus::setInReplyToUserId(qint64 id)
{
    m_statusInfo.insert(QTweetStatus::InReplyToUserId, id);
}

qint64 QTweetStatus::inReplyToUserId() const
{
    return m_statusInfo.value(QTweetStatus::InReplyToUserId).toLongLong();
}

void QTweetStatus::setInReplyToScreenName(const QString &screenName)
{
    m_statusInfo.insert(QTweetStatus::InReplyToScreenName, screenName);
}

QString QTweetStatus::inReplyToScreenName() const
{
    return m_statusInfo.value(QTweetStatus::InReplyToScreenName).toString();
}

void QTweetStatus::setUser(const QTweetUser &user)
{
    QVariant userVariant;
    userVariant.setValue(user);

    m_statusInfo.insert(QTweetStatus::User, userVariant);
}

QTweetUser QTweetStatus::user() const
{
    QVariant user = m_statusInfo.value(QTweetStatus::User);

    return user.value<QTweetUser>();
}

void QTweetStatus::setRetweetedStatus(const QTweetStatus &status)
{
    QVariant rtStatus;
    rtStatus.setValue(status);

    m_statusInfo.insert(QTweetStatus::RetweetedStatus, rtStatus);
}

QTweetStatus QTweetStatus::retweetedStatus() const
{
    QVariant rtStatus = m_statusInfo.value(QTweetStatus::RetweetedStatus);

    return rtStatus.value<QTweetStatus>();
}
