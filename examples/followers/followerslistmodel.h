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

#ifndef FOLLOWERSLISTMODEL_H
#define FOLLOWERSLISTMODEL_H

#include <QAbstractListModel>
#include "qtweetuser.h"

class OAuthTwitter;

class FollowersListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        NameRole = Qt::UserRole + 1,
        ScreenNameRole,
        DescriptionRole,
        AvatarRole
    };

    FollowersListModel(QObject *parent = 0);
    FollowersListModel(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void setOAuthTwitter(OAuthTwitter *oauthTwitter);
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void fetchFollowers(const QString& cursor = QString("-1"));

private slots:
    void followersFinished(const QList<QTweetUser>& followers,
                           const QString& nextCursor);

private:
    OAuthTwitter *m_oauthTwitter;
    QList<QTweetUser> m_users;
};

#endif // FOLLOWERSLISTMODEL_H
