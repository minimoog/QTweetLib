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

#ifndef QTWEETDMSTATUS_H
#define QTWEETDMSTATUS_H

#include <QVariant>
#include <QSharedDataPointer>
#include "qtweetlib_global.h"

class QTweetUser;
class QTweetDMStatusData;

/**
 *   Stores direct message info
 */
class QTWEETLIBSHARED_EXPORT QTweetDMStatus
{
public:
    QTweetDMStatus();
    QTweetDMStatus(const QTweetDMStatus& other);
    QTweetDMStatus& operator=(const QTweetDMStatus& other);
    ~QTweetDMStatus();

    void setCreatedAt(const QString& twitterDate);
    void setCreatedAt(const QDateTime& datetime);
    QDateTime createdAt() const;
    void setSenderScreenName(const QString& screenName);
    QString senderScreenName() const;
    void setSender(const QTweetUser& sender);
    QTweetUser sender() const;
    void setText(const QString& text);
    QString text() const;
    void setRecipientScreenName(const QString& screenName);
    QString recipientScreenName() const;
    void setId(qint64 id);
    qint64 id() const;
    void setRecipient(const QTweetUser& recipient);
    QTweetUser recipient() const;
    void setRecipientId(qint64 id);
    qint64 recipientId() const;
    void setSenderId(qint64 id);
    qint64 senderId() const;

private:
    QSharedDataPointer<QTweetDMStatusData> d;
};

Q_DECLARE_METATYPE(QTweetDMStatus)

#endif // QTWEETDMSTATUS_H
