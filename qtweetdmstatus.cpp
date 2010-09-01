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
#include "qtweetdmstatus.h"

QTweetDMStatus::QTweetDMStatus()
{
}

void QTweetDMStatus::setCreatedAt(const QString &twitterDate)
{
    QDateTime datetime = QTweetUser::twitterDateToQDateTime(twitterDate);

    m_dmInfo.insert(QTweetDMStatus::CreatedAt, datetime);
}

QDateTime QTweetDMStatus::createdAt() const
{
    return m_dmInfo.value(QTweetDMStatus::CreatedAt).toDateTime();
}

void QTweetDMStatus::setSenderScreenName(const QString &screenName)
{
    m_dmInfo.insert(QTweetDMStatus::SenderScreenName, screenName);
}

QString QTweetDMStatus::senderScreenName() const
{
    return m_dmInfo.value(QTweetDMStatus::SenderScreenName).toString();
}

void QTweetDMStatus::setSender(const QTweetUser &sender)
{
    QVariant senderVariant;
    senderVariant.setValue(sender);

    m_dmInfo.insert(QTweetDMStatus::Sender, senderVariant);
}

QTweetUser QTweetDMStatus::sender() const
{
    QVariant senderVariant = m_dmInfo.value(QTweetDMStatus::Sender);

    return senderVariant.value<QTweetUser>();
}

void QTweetDMStatus::setText(const QString &text)
{
    m_dmInfo.insert(QTweetDMStatus::Text, text);
}

QString QTweetDMStatus::text() const
{
    return m_dmInfo.value(QTweetDMStatus::Text).toString();
}

void QTweetDMStatus::setRecipientScreenName(const QString &screenName)
{
    m_dmInfo.insert(QTweetDMStatus::RecipientScreenName, screenName);
}

QString QTweetDMStatus::recipientScreenName() const
{
    return m_dmInfo.value(QTweetDMStatus::RecipientScreenName).toString();
}

void QTweetDMStatus::setId(qint64 id)
{
    m_dmInfo.insert(QTweetDMStatus::Id, id);
}

qint64 QTweetDMStatus::id() const
{
    return m_dmInfo.value(QTweetDMStatus::Id).toLongLong();
}

void QTweetDMStatus::setRecipient(const QTweetUser &recipient)
{
    QVariant recipientVariant;
    recipientVariant.setValue(recipient);

    m_dmInfo.insert(QTweetDMStatus::Recipient, recipientVariant);
}

QTweetUser QTweetDMStatus::recipient() const
{
    QVariant recipientVariant = m_dmInfo.value(QTweetDMStatus::Recipient);

    return recipientVariant.value<QTweetUser>();
}

void QTweetDMStatus::setRecipientId(qint64 id)
{
    m_dmInfo.insert(QTweetDMStatus::RecipientId, id);
}

qint64 QTweetDMStatus::recipientId() const
{
    return m_dmInfo.value(QTweetDMStatus::RecipientId).toLongLong();
}

void QTweetDMStatus::setSenderId(qint64 id)
{
    m_dmInfo.insert(QTweetDMStatus::SenderId, id);
}

qint64 QTweetDMStatus::senderId() const
{
    return m_dmInfo.value(QTweetDMStatus::SenderId).toLongLong();
}
