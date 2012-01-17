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

#ifndef QTWEETBLOCKSEXISTS_H
#define QTWEETBLOCKSEXISTS_H

#include "qtweetnetbase.h"

/**
 *  Emits finishedIsBlocked if the authenticating user is blocking a target user.
 *  Will return the blocked user's object if a block exists,
 *  and error with a HTTP 404 response code otherwise.
 */
class QTweetBlocksExists : public QTweetNetBase
{
    Q_OBJECT
public:
    explicit QTweetBlocksExists(QObject *parent = 0);
    QTweetBlocksExists(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void isBlocked(qint64 userid, bool includeEntities = false);
    void isBlocked(const QString& screenName, bool includeEntities = false);

signals:
    void finishedIsBlocked(const QTweetUser& user);

protected:
    void parseJsonFinished(cJSON *root);
};

#endif // QTWEETBLOCKSEXISTS_H
