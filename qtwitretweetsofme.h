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

#ifndef QTWITRETWEETSOFME_H
#define QTWITRETWEETSOFME_H

#include "qtwitnetbase.h"

class QTWITLIBSHARED_EXPORT QTwitRetweetsOfMe : public QTwitNetBase
{
    Q_OBJECT
public:
    QTwitRetweetsOfMe(QObject *parent = 0);
    QTwitRetweetsOfMe(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void fetch(ResponseType respType = QTwitNetBase::JSON,
               qint64 sinceid = 0,
               qint64 maxid = 0,
               int count = 0,
               int page = 0);

private slots:
    void reply();
    void error();
};

#endif // QTWITRETWEETSOFME_H
