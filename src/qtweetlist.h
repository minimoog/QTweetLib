/* Copyright (c) 2010, Antonie Jovanoski
 *
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact e-mail: Antonie Jovanoski <minimoog77_at_gmail.com>
 */

#ifndef QTWEETLIST_H
#define QTWEETLIST_H

#include <QVariant>
#include <QHash>
#include "qtweetlib_global.h"

class QTweetUser;

class QTWEETLIBSHARED_EXPORT QTweetList
{
public:
    QTweetList();

    enum TypeInfo {
        Mode,
        Description,
        Following,
        MemberCount,
        FullName,
        SubscriberCount,
        Slug,
        Name,
        Id,
        Uri,
        User
    };

    void setMode(const QString& mode) { m_listInfo.insert(QTweetList::Mode, mode); }
    QString mode() const { return m_listInfo.value(QTweetList::Mode).toString(); }
    void setDescription(const QString& desc) { m_listInfo.insert(QTweetList::Description, desc); }
    QString description() const { return m_listInfo.value(QTweetList::Description).toString(); }
    void setFollowing(bool following) { m_listInfo.insert(QTweetList::Following, following); }
    bool following() const { return m_listInfo.value(QTweetList::Following).toBool(); }
    void setMemberCount(int count) { m_listInfo.insert(QTweetList::MemberCount, count); }
    int memberCount() const { return m_listInfo.value(QTweetList::MemberCount).toInt(); }
    void setFullName(const QString& name) { m_listInfo.insert(QTweetList::FullName, name); }
    QString fullName() const { return m_listInfo.value(QTweetList::FullName).toString(); }
    void setSubscriberCount(int count) { m_listInfo.insert(QTweetList::SubscriberCount, count); }
    int subscriberCount() const { return m_listInfo.value(QTweetList::SubscriberCount).toInt(); }
    void setSlug(const QString& slug) { m_listInfo.insert(QTweetList::Slug, slug); }
    QString slug() const { return m_listInfo.value(QTweetList::Slug).toString(); }
    void setName(const QString& name) { m_listInfo.insert(QTweetList::Name, name); }
    QString name() const { return m_listInfo.value(QTweetList::Name).toString(); }
    void setId(qint64 id) { m_listInfo.insert(QTweetList::Id, id); }
    qint64 id() const { return m_listInfo.value(QTweetList::Id).toLongLong(); }
    void setUri(const QString& uri) { m_listInfo.insert(QTweetList::Uri, uri); }
    QString uri() const { return m_listInfo.value(QTweetList::Uri).toString(); }
    void setUser(const QTweetUser& user);
    QTweetUser user() const;

private:
    QHash<int, QVariant> m_listInfo;
};

Q_DECLARE_METATYPE(QTweetList)

#endif // QTWEETLIST_H
