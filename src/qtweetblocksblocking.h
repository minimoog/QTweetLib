/* Copyright (c) 2011, Antonie Jovanoski
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

#ifndef QTWEETBLOCKSBLOCKING_H
#define QTWEETBLOCKSBLOCKING_H

#include "qtweetnetbase.h"

/**
 *  Emits the list of users that the authenticating user is blocking.
 */
class QTWEETLIBSHARED_EXPORT QTweetBlocksBlocking : public QTweetNetBase
{
    Q_OBJECT
public:
    explicit QTweetBlocksBlocking(QObject *parent = 0);
    QTweetBlocksBlocking(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void getBlocks(int page = 0, bool includeEntities = false);

signals:
    void finishedGettingBlocks(const QList<QTweetUser>& users);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};

#endif // QTWEETBLOCKSBLOCKING_H
