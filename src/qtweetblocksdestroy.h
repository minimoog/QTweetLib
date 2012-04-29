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

#ifndef QTWEETBLOCKSDESTROY_H
#define QTWEETBLOCKSDESTROY_H

#include "qtweetnetbase.h"

/**
 *  Un-blocks the user specified in the ID parameter for the authenticating user.
 *  Returns the un-blocked user when successful.
 */
class QTWEETLIBSHARED_EXPORT QTweetBlocksDestroy : public QTweetNetBase
{
    Q_OBJECT
public:
    explicit QTweetBlocksDestroy(QObject *parent = 0);
    QTweetBlocksDestroy(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void unblock(qint64 userid, bool includeEntities = false);
    void unblock(const QString& screenName, bool includeEntities = false);

signals:
    /** Emits the ublocked user */
    void finishedUnblocking(const QTweetUser& user);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};

#endif // QTWEETBLOCKSDESTROY_H
