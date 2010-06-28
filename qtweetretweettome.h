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

#ifndef QTWEETRETWEETTOME_H
#define QTWEETRETWEETTOME_H

#include "qtweetnetbase.h"

class QTWEETLIBSHARED_EXPORT QTweetRetweetToMe : public QTweetNetBase
{
    Q_OBJECT
public:
    QTweetRetweetToMe(QObject *parent = 0);
    QTweetRetweetToMe(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void fetch(ResponseType respType,
               qint64 sinceid,
               qint64 maxid,
               int count,
               int page);

private slots:
    void reply();
    void error();
};

#endif // QTWEETRETWEETTOME_H
