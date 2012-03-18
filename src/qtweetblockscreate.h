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

#ifndef QTWEETBLOCKSCREATE_H
#define QTWEETBLOCKSCREATE_H

#include "qtweetnetbase.h"

/**
 *  Blocks the user specified in the ID parameter as the authenticating user.
 *  Destroys a friendship to the blocked user if it exists.
 *  Returns the blocked user when successful.
 */
class QTWEETLIBSHARED_EXPORT QTweetBlocksCreate : public QTweetNetBase
{
    Q_OBJECT
public:
    explicit QTweetBlocksCreate(QObject *parent = 0);
    QTweetBlocksCreate(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void create(qint64 userid, bool includeEnttities = false);
    void create(const QString& screenName, bool includeEntities = false);

signals:
    /** Emits the blocked user */
    void finishedCreatingBlock(const QTweetUser& user);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};

#endif // QTWEETBLOCKSCREATE_H
