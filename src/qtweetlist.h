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

#ifndef QTWEETLIST_H
#define QTWEETLIST_H

#include <QVariant>
#include <QSharedDataPointer>
#include "qtweetlib_global.h"

class QTweetUser;
class QTweetListData;

/**
 *  Stores list information
 */
class QTWEETLIBSHARED_EXPORT QTweetList
{
public:
    QTweetList();
    QTweetList(const QTweetList& other);
    QTweetList& operator=(const QTweetList& other);
    ~QTweetList();

    void setMode(const QString& mode);
    QString mode() const;
    void setDescription(const QString& desc);
    QString description() const;
    void setFollowing(bool following);
    bool following() const;
    void setMemberCount(int count);
    int memberCount() const;
    void setFullName(const QString& name);
    QString fullName() const;
    void setSubscriberCount(int count);
    int subscriberCount() const;
    void setSlug(const QString& slug);
    QString slug() const;
    void setName(const QString& name);
    QString name() const;
    void setId(qint64 id);
    qint64 id() const;
    void setUri(const QString& uri);
    QString uri() const;
    void setUser(const QTweetUser& user);
    QTweetUser user() const;

private:
    QSharedDataPointer<QTweetListData> d;
};

Q_DECLARE_METATYPE(QTweetList)

#endif // QTWEETLIST_H
