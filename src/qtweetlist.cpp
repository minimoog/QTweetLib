/* Copyright 2010 Antonie Jovanoski
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Contact e-mail: Antonie Jovanoski <minimoog77_at_gmail.com>
 */

#include <QSharedData>
#include "qtweetlist.h"
#include "qtweetuser.h"

class QTweetListData : public QSharedData
{
public:
    QString mode;
    QString description;
    bool following;
    int memberCount;
    QString fullname;
    int subscriberCount;
    QString slug;
    QString name;
    qint64 id;
    QString uri;
    QTweetUser user;
};

QTweetList::QTweetList() :
        d(new QTweetListData)
{
}

QTweetList::QTweetList(const QTweetList &other) :
        d(other.d)
{
}

QTweetList& QTweetList::operator =(const QTweetList& other)
{
   if (this != &other)
       d.operator =(other.d);
   return *this;
}

QTweetList::~QTweetList()
{
}

void QTweetList::setMode(const QString &mode)
{
    d->mode = mode;
}

QString QTweetList::mode() const
{
    return d->mode;
}

void QTweetList::setDescription(const QString &desc)
{
    d->description = desc;
}

QString QTweetList::description() const
{
    return d->description;
}

void QTweetList::setFollowing(bool following)
{
    d->following = following;
}

bool QTweetList::following() const
{
    return d->following;
}

void QTweetList::setMemberCount(int count)
{
    d->memberCount = count;
}

int QTweetList::memberCount() const
{
    return d->memberCount;
}

void QTweetList::setFullName(const QString &name)
{
    d->fullname = name;
}

QString QTweetList::fullName() const
{
    return d->fullname;
}

void QTweetList::setSubscriberCount(int count)
{
    d->subscriberCount = count;
}

int QTweetList::subscriberCount() const
{
    return d->subscriberCount;
}

void QTweetList::setSlug(const QString &slug)
{
    d->slug = slug;
}

QString QTweetList::slug() const
{
    return d->slug;
}

void QTweetList::setName(const QString &name)
{
    d->name = name;
}

QString QTweetList::name() const
{
    return d->name;
}

void QTweetList::setId(qint64 id)
{
    d->id = id;
}

qint64 QTweetList::id() const
{
    return d->id;
}

void QTweetList::setUri(const QString &uri)
{
    d->uri = uri;
}

QString QTweetList::uri() const
{
    return d->uri;
}

void QTweetList::setUser(const QTweetUser &user)
{
    d->user = user;
}

QTweetUser QTweetList::user() const
{
    return d->user;
}
