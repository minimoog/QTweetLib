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

#include "qtwituser.h"
#include <QSharedData>
#include <QColor>

class QTwitUserData : public QSharedData {
public:
    QTwitUserData() : id(0) {}

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

QTwitUser::QTwitUser() : data(new QTwitUserData)
{
}

QTwitUser::QTwitUser(const QTwitUser &rhs) : data(rhs.data)
{
}

QTwitUser &QTwitUser::operator=(const QTwitUser &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

QTwitUser::~QTwitUser()
{
}

void QTwitUser::setId(qint64 id)
{
    data->id = id;
}

qint64 QTwitUser::id() const
{
    return data->id;
}

void QTwitUser::setName(const QString &name)
{
    data->name = name;
}

QString QTwitUser::name() const
{
    return data->name;
}

void QTwitUser::setScreenName(const QString &screenName)
{
    data->screenName = screenName;
}

QString QTwitUser::screenName() const
{
    return data->screenName;
}

void QTwitUser::setprofileImageUrl(const QString &url)
{
    data->profileImageUrl = url;
}

QString QTwitUser::profileImageUrl() const
{
    return data->profileImageUrl;
}


