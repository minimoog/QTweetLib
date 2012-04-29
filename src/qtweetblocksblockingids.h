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

#ifndef QTWEETBLOCKSBLOCKINGIDS_H
#define QTWEETBLOCKSBLOCKINGIDS_H

#include "qtweetnetbase.h"

/**
 *  Emits a list of numeric user ids the authenticating user is blocking.
 */
class QTWEETLIBSHARED_EXPORT QTweetBlocksBlockingIDs : public QTweetNetBase
{
    Q_OBJECT
public:
    explicit QTweetBlocksBlockingIDs(QObject *parent = 0);
    QTweetBlocksBlockingIDs(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void getIDs();

signals:
    void finishedGettingIDs(const QList<qint64> useridlist);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};

#endif // QTWEETBLOCKSBLOCKINGIDS_H
