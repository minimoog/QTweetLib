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

#ifndef QTWITTERSTATUSSHOW_H
#define QTWITTERSTATUSSHOW_H

#include "qtwitternetbase.h"

class QTWITTERLIBSHARED_EXPORT QtwitterStatusShow : public QtwitterNetBase
{
    Q_OBJECT
public:
    QtwitterStatusShow(QObject *parent = 0);
    QtwitterStatusShow(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void fetch(qint64 id, ResponseType respType = QtwitterNetBase::JSON);

private slots:
    void reply();
    void error();
};

#endif // QTWITTERSTATUSSHOW_H
