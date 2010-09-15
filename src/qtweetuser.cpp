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

#include <QDateTime>
#include "qtweetuser.h"
#include "qtweetstatus.h"

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

void QTweetUser::setLocation(const QString &location)
{
    m_userInfo.insert(QTweetUser::Location, location);
}

QString QTweetUser::location() const
{
    return m_userInfo.value(QTweetUser::Location).toString();
}

void QTweetUser::setDescription(const QString &desc)
{
    m_userInfo.insert(QTweetUser::Description, desc);
}

QString QTweetUser::description() const
{
    return m_userInfo.value(QTweetUser::Description).toString();
}

void QTweetUser::setprofileImageUrl(const QString &url)
{
    m_userInfo.insert(QTweetUser::ProfileImageUrl, url);
}

QString QTweetUser::profileImageUrl() const
{
    return m_userInfo.value(QTweetUser::ProfileImageUrl).toString();
}

void QTweetUser::setUrl(const QString &url)
{
    m_userInfo.insert(QTweetUser::Url, url);
}

QString QTweetUser::url() const
{
    return m_userInfo.value(QTweetUser::Url).toString();
}

void QTweetUser::setProtected(bool protected_)
{
    m_userInfo.insert(QTweetUser::Protected, protected_);
}

bool QTweetUser::isProtected() const
{
    return m_userInfo.value(QTweetUser::Protected).toBool();
}

void QTweetUser::setFollowersCount(int count)
{
    m_userInfo.insert(QTweetUser::FollowersCount, count);
}

int QTweetUser::followersCount() const
{
    return m_userInfo.value(QTweetUser::FollowersCount).toInt();
}

void QTweetUser::setFriendsCount(int count)
{
    m_userInfo.insert(QTweetUser::FriendsCount, count);
}

int QTweetUser::friendsCount() const
{
    return m_userInfo.value(QTweetUser::FriendsCount).toInt();
}

void QTweetUser::setCreatedAt(const QString &twitterDate)
{
    QDateTime datetime = twitterDateToQDateTime(twitterDate);

    m_userInfo.insert(QTweetUser::CreatedAt, datetime);
}

QDateTime QTweetUser::createdAt() const
{
    return m_userInfo.value(QTweetUser::CreatedAt).toDateTime();
}

void QTweetUser::setFavouritesCount(int count)
{
    m_userInfo.insert(QTweetUser::FavouritesCount, count);
}

int QTweetUser::favouritesCount() const
{
    return m_userInfo.value(QTweetUser::FavouritesCount).toInt();
}

void QTweetUser::setUtcOffset(int sec)
{
    m_userInfo.insert(QTweetUser::UtcOffset, sec);
}

int QTweetUser::utcOffset() const
{
    return m_userInfo.value(QTweetUser::UtcOffset).toInt();
}

void QTweetUser::setTimezone(const QString &timezone)
{
    m_userInfo.insert(QTweetUser::TimeZone, timezone);
}

QString QTweetUser::timezone() const
{
    return m_userInfo.value(QTweetUser::TimeZone).toString();
}

void QTweetUser::setGeoEnabled(bool isGeoEnabled)
{
    m_userInfo.insert(QTweetUser::GeoEnabled, isGeoEnabled);
}

bool QTweetUser::isGeoEnabled() const
{
    return m_userInfo.value(QTweetUser::GeoEnabled, false).toBool();
}

void QTweetUser::setVerified(bool verified)
{
    m_userInfo.insert(QTweetUser::Verified, verified);
}

bool QTweetUser::isVerified() const
{
    return m_userInfo.value(QTweetUser::Verified, false).toBool();
}

void QTweetUser::setFollowing(bool following)
{
    m_userInfo.insert(QTweetUser::Following, following);
}

bool QTweetUser::isFollowing() const
{
    return m_userInfo.value(QTweetUser::Following, false).toBool();
}

void QTweetUser::setStatusesCount(int count)
{
    m_userInfo.insert(QTweetUser::StatusesCount, count);
}

int QTweetUser::statusesCount() const
{
    return m_userInfo.value(QTweetUser::StatusesCount).toInt();
}

void QTweetUser::setStatus(const QTweetStatus &lastStatus)
{
    QVariant statusVariant;
    statusVariant.setValue(lastStatus);

    m_userInfo.insert(QTweetUser::Status, statusVariant);
}

QTweetStatus QTweetUser::status() const
{
    QVariant status = m_userInfo.value(QTweetUser::Status);

    return status.value<QTweetStatus>();
}

QDateTime QTweetUser::twitterDateToQDateTime(const QString &twitterDate)
{
    //Twitter Date Format: 'Wed Sep 01 11:27:25 +0000 2010'  UTC
    QString dateString = twitterDate.left(11) + twitterDate.right(4);
    QString timeString = twitterDate.mid(11, 8);

    QDate date = QDate::fromString(dateString);
    QTime time = QTime::fromString(timeString);

    if (date.isValid() && time.isValid())
        return QDateTime(date, time, Qt::UTC);
    else
        return QDateTime();
}
