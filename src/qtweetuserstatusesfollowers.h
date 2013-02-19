/* Copyright 2010 Antonie Jovanoski
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Contact e-mail: Antonie Jovanoski <minimoog77_at_gmail.com>
 */

#ifndef QTWEETUSERSTATUSESFOLLOWERS_H
#define QTWEETUSERSTATUSESFOLLOWERS_H

#include "qtweetnetbase.h"

/**
 *  Fetches the authenticating user's followers, each with current status inline.
 *  They are ordered by the order in which they followed the user, 100 at a time.
 *
 *   Use the cursor option to access older friends.
 */
class QTWEETLIBSHARED_EXPORT QTweetUserStatusesFollowers : public QTweetNetBase
{
    Q_OBJECT
public:
    QTweetUserStatusesFollowers(QObject *parent = 0);
    QTweetUserStatusesFollowers(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void fetch(qint64 userid = 0,
               const QString& cursor = QString(),
               bool includeEntities = false);
    void fetch(const QString& screenName = QString(),
               const QString& cursor = QString(),
               bool includeEntities = false);

signals:
    /** Emits page of followers list
     *  @param followersList list of friends
     *  @param nextCursor cursor for next page, "0" if there is no next page, empty if there is no paging
     *  @param prevCursor cursor for prev page, "0" if there is no prev page, empty if there is no paging
     */
    void parsedFollowersList(const QList<QTweetUser>& followersList,
                             const QString& nextCursor = QString(),
                             const QString& prevCursor = QString());

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);

private:
    bool m_usesCursoring;
};

#endif // QTWEETUSERSTATUSESFOLLOWERS_H
