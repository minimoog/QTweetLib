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

#include <QSharedData>
#include <QDateTime>
#include "qtweetstatus.h"
#include "qtweetuser.h"

class QTweetStatusData : public QSharedData
{
public:
    QTweetStatusData() : id(0), inReplyToStatusId(0), containsRetweetStatus(false) {}

    qint64 id;
    QString text;
    QDateTime createdAt;
    qint64 inReplyToUserId;
    QString inReplyToScreenName;
    qint64 inReplyToStatusId;
    bool favorited;
    QString source;
    QTweetUser user;
    //avoid recursion
    //QTweetStatus retweetedStatus;
    qint64 rtsId;
    QString rtsText;
    QDateTime rtsCreatedAt;
    qint64 rtsInReplyToUserId;
    QString rtsInReplyToScreenName;
    qint64 rtsInReplyToStatusId;
    bool rtsFavorited;
    QString rtsSource;
    QTweetUser rtsUser;
    bool containsRetweetStatus;
};

QTweetStatus::QTweetStatus() :
        d(new QTweetStatusData)
{
}

QTweetStatus::QTweetStatus(const QTweetStatus &other) :
        d(other.d)
{
}

QTweetStatus& QTweetStatus::operator=(const QTweetStatus &rhs)
{
    if (this != &rhs)
        d.operator=(rhs.d);
    return *this;
}

QTweetStatus::~QTweetStatus()
{
}

void QTweetStatus::setId(qint64 id)
{
    d->id = id;
}

qint64 QTweetStatus::id() const
{
    return d->id;
}

void QTweetStatus::setText(const QString &text)
{
    d->text = text;
}

QString QTweetStatus::text() const
{
    return d->text;
}

void QTweetStatus::setCreatedAt(const QString &twitterDate)
{
    d->createdAt = QTweetUser::twitterDateToQDateTime(twitterDate);
}

void QTweetStatus::setCreatedAt(const QDateTime &dateTime)
{
    d->createdAt = dateTime;
}

QDateTime QTweetStatus::createdAt() const
{
    return d->createdAt;
}

void QTweetStatus::setInReplyToUserId(qint64 id)
{
    d->inReplyToUserId = id;
}

qint64 QTweetStatus::inReplyToUserId() const
{
    return d->inReplyToUserId;
}

void QTweetStatus::setInReplyToScreenName(const QString &screenName)
{
    d->inReplyToScreenName = screenName;
}

QString QTweetStatus::inReplyToScreenName() const
{
    return d->inReplyToScreenName;
}

void QTweetStatus::setInReplyToStatusId(qint64 id)
{
    d->inReplyToStatusId = id;
}

qint64 QTweetStatus::inReplyToStatusId() const
{
    return d->inReplyToStatusId;
}

void QTweetStatus::setFavorited(bool fav)
{
    d->favorited = fav;
}

bool QTweetStatus::favorited() const
{
    return d->favorited;
}

void QTweetStatus::setSource(const QString &source)
{
    d->source = source;
}

QString QTweetStatus::source() const
{
    return d->source;
}

void QTweetStatus::setUser(const QTweetUser &user)
{
    d->user = user;
}

QTweetUser QTweetStatus::user() const
{
    return d->user;
}

void QTweetStatus::setRetweetedStatus(const QTweetStatus &status)
{
    //recursion? circural reference?
    d->rtsId = status.id();
    d->rtsText = status.text();
    d->rtsCreatedAt = status.createdAt();
    d->rtsInReplyToUserId = status.inReplyToUserId();
    d->rtsInReplyToStatusId = status.inReplyToUserId();
    d->rtsFavorited = status.favorited();
    d->rtsSource = status.source();
    d->rtsUser = status.user();
    d->containsRetweetStatus = true;
}

QTweetStatus QTweetStatus::retweetedStatus() const
{
    QTweetStatus status;
    status.setId(d->rtsId);
    status.setText(d->rtsText);
    //status.setCreatedAt(d->rtsCreatedAt); // ### TODO FIX
    status.setInReplyToUserId(d->rtsInReplyToUserId);
    status.setInReplyToScreenName(d->rtsInReplyToScreenName);
    status.setInReplyToStatusId(d->rtsInReplyToStatusId);
    status.setFavorited(d->rtsFavorited);
    status.setSource(d->rtsSource);
    status.setUser(d->rtsUser);

    return status;
}

bool QTweetStatus::isRetweet() const
{
    return d->containsRetweetStatus;
}
