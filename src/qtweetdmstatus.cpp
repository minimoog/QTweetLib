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
#include <QDateTime>
#include "qtweetuser.h"
#include "qtweetdmstatus.h"

class QTweetDMStatusData : public QSharedData
{
public:
    QDateTime createdAt;
    QString senderScreenName;
    QTweetUser sender;
    QString text;
    QString recipientScreenName;
    qint64 id;
    QTweetUser recipient;
    qint64 recipientId;
    qint64 senderId;
};

QTweetDMStatus::QTweetDMStatus() :
        d(new QTweetDMStatusData)
{
}

QTweetDMStatus::QTweetDMStatus(const QTweetDMStatus &other) :
        d(other.d)
{
}

QTweetDMStatus& QTweetDMStatus::operator =(const QTweetDMStatus &other)
{
    if (this != &other)
        d.operator =(other.d);
    return *this;
}

QTweetDMStatus::~QTweetDMStatus()
{
}

void QTweetDMStatus::setCreatedAt(const QString &twitterDate)
{
    d->createdAt = QTweetUser::twitterDateToQDateTime(twitterDate);
}

void QTweetDMStatus::setCreatedAt(const QDateTime &datetime)
{
    d->createdAt = datetime;
}

QDateTime QTweetDMStatus::createdAt() const
{
    return d->createdAt;
}

void QTweetDMStatus::setSenderScreenName(const QString &screenName)
{
    d->senderScreenName = screenName;
}

QString QTweetDMStatus::senderScreenName() const
{
    return d->senderScreenName;
}

void QTweetDMStatus::setSender(const QTweetUser &sender)
{
    d->sender = sender;
}

QTweetUser QTweetDMStatus::sender() const
{
    return d->sender;
}

void QTweetDMStatus::setText(const QString &text)
{
    d->text = text;
}

QString QTweetDMStatus::text() const
{
    return d->text;
}

void QTweetDMStatus::setRecipientScreenName(const QString &screenName)
{
    d->recipientScreenName = screenName;
}

QString QTweetDMStatus::recipientScreenName() const
{
    return d->recipientScreenName;
}

void QTweetDMStatus::setId(qint64 id)
{
    d->id = id;
}

qint64 QTweetDMStatus::id() const
{
    return d->id;
}

void QTweetDMStatus::setRecipient(const QTweetUser &recipient)
{
    d->recipient = recipient;
}

QTweetUser QTweetDMStatus::recipient() const
{
    return d->recipient;
}

void QTweetDMStatus::setRecipientId(qint64 id)
{
    d->recipientId = id;
}

qint64 QTweetDMStatus::recipientId() const
{
    return d->recipientId;
}

void QTweetDMStatus::setSenderId(qint64 id)
{
    d->senderId = id;
}

qint64 QTweetDMStatus::senderId() const
{
    return d->senderId;
}
