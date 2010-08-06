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

#include "qtweetuser.h"
#include <QColor>

QTweetUser::QTweetUser()
{
}


void QTweetUser::setId(qint64 id)
{
    m_userInfo.insert(QTweetUser::Id, id);
}

qint64 QTweetUser::id() const
{
    return m_userInfo.value(QTweetUser::Id).toLongLong();
}

void QTweetUser::setName(const QString &name)
{
    m_userInfo.insert(QTweetUser::Name, name);
}

QString QTweetUser::name() const
{
    return m_userInfo.value(QTweetUser::Name).toString();
}

void QTweetUser::setScreenName(const QString &screenName)
{
    m_userInfo.insert(QTweetUser::ScreenName, screenName);
}

QString QTweetUser::screenName() const
{
    return m_userInfo.value(QTweetUser::ScreenName).toString();
}

void QTweetUser::setprofileImageUrl(const QString &url)
{
    m_userInfo.insert(QTweetUser::ProfileImageUrl, url);
}

QString QTweetUser::profileImageUrl() const
{
    return m_userInfo.value(QTweetUser::ProfileImageUrl).toString();
}


