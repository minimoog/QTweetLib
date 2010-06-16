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

#ifndef QTWITTERFRIENDSTIMELINE_H
#define QTWITTERFRIENDSTIMELINE_H

#include "QtwitterLib_global.h"
#include "qtwitternetbase.h"

class QtwitterFriendsTimeline : public QtwitterNetBase
{
    Q_OBJECT
public:
    QtwitterFriendsTimeline(QObject *parent = 0);
    QtwitterFriendsTimeline(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void fetch(ResponseType respType = QtwitterNetBase::JSON,
               qint64 sinceid = 0,
               qint64 maxid = 0,
               int count = 0,
               int page = 0,
               bool skipUser = false,
               bool includeRts = false,
               bool includeEntities = false);
    QByteArray response() const;

private slots:
    void reply();
    void error();

private:
    QByteArray m_response;
};

#endif // QTWITTERFRIENDSTIMELINE_H
