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
#include <QSharedData>
#include <QColor>

class QTweetUserData : public QSharedData {
public:
    QTweetUserData() : id(0) {}

    qint64 id;
    QString name;
    QString screenName;
    QString location;                   //
    QString description;                //
    QString profileImageUrl;
    QString url;                        //
    bool protected_r;                   //
    int followersCount;                 //
    QColor profileBackgroundColor;      //
    QColor profileTextColor;            //
    QColor profileLinkColor;            //
    QColor profileSidebarFillColor;     //
    QColor profileSidebarBorderColor;   //
    int friendsCount;                   //
    QString createdAt; // ### TODO: Use QDateTime
    int favouritesCount;                //
    int utcOffset;                      //
    QString timeZone;                   //
    QString profileBackgroundImageUrl;  //
    bool profileBackgroundTile;         //
    bool notifications;                 //
    bool geoEnabled;                    //
    bool verified;                      //
    bool following;                     //
    int statusesCount;                  //
    QString lang;                       //
    bool contributors_enabled;          //
};

QTweetUser::QTweetUser() : data(new QTweetUserData)
{
}

QTweetUser::QTweetUser(const QTweetUser &rhs) : data(rhs.data)
{
}

QTweetUser &QTweetUser::operator=(const QTweetUser &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

QTweetUser::~QTweetUser()
{
}

void QTweetUser::setId(qint64 id)
{
    data->id = id;
}

qint64 QTweetUser::id() const
{
    return data->id;
}

void QTweetUser::setName(const QString &name)
{
    data->name = name;
}

QString QTweetUser::name() const
{
    return data->name;
}

void QTweetUser::setScreenName(const QString &screenName)
{
    data->screenName = screenName;
}

QString QTweetUser::screenName() const
{
    return data->screenName;
}

void QTweetUser::setprofileImageUrl(const QString &url)
{
    data->profileImageUrl = url;
}

QString QTweetUser::profileImageUrl() const
{
    return data->profileImageUrl;
}


