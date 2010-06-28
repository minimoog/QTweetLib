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

#ifndef QTWEETUSER_H
#define QTWEETUSER_H

#include <QSharedDataPointer>

class QTweetUserData;

class QTweetUser
{
public:
    QTweetUser();
    QTweetUser(const QTweetUser &);
    QTweetUser &operator=(const QTweetUser &);
    ~QTweetUser();

    void setId(qint64 id);
    qint64 id() const;
    void setName(const QString& name);
    QString name() const;
    void setScreenName(const QString& screenName);
    QString screenName() const;
    void setprofileImageUrl(const QString& url);
    QString profileImageUrl() const;

private:
    QSharedDataPointer<QTweetUserData> data;
};

#endif // QTWEETUSER_H
