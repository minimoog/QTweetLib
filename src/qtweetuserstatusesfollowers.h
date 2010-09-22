/* Copyright (c) 2010, Antonie Jovanoski
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

#ifndef QTWEETUSERSTATUSESFOLLOWERS_H
#define QTWEETUSERSTATUSESFOLLOWERS_H

#include "qtweetnetbase.h"

/*!
  Fetches the authenticating user's followers, each with current status inline.
  They are ordered by the order in which they followed the user, 100 at a time.

  Use the cursor option to access older friends.
 */
class QTWEETLIBSHARED_EXPORT QTweetUserStatusesFollowers : public QTweetNetBase
{
    Q_OBJECT
public:
    QTweetUserStatusesFollowers(QObject *parent = 0);
    QTweetUserStatusesFollowers(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void fetch(qint64 userid = 0,
               const QString& screenName = QString(),
               const QString& cursor = QString(),
               bool includeEntities = false,
               ResponseType respType = QTweetNetBase::JSON);

signals:
    /*! Emited when json parsing is finished
        \param followersList List of friends
        \param nextCursor Cursor for next page, "0" if there is no next page, empty if there is no paging
        \param prevCursor Cursor for prev page, "0" if there is no prev page, empty if there is no paging
     */
    void parsedFollowersList(const QList<QTweetUser>& followersList,
                             const QString& nextCursor = QString(),
                             const QString& prevCursor = QString());

protected slots:
    void parsingJsonFinished(const QVariant &json, bool ok, const QString &errorMsg);

private slots:
    void error();

private:
    bool m_usesCursoring;
};

#endif // QTWEETUSERSTATUSESFOLLOWERS_H
