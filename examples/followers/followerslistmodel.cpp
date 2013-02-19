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

#include "followerslistmodel.h"
#include "oauthtwitter.h"
#include "qtweetuser.h"
#include "qtweetuserstatusesfollowers.h"

FollowersListModel::FollowersListModel(QObject *parent) :
    QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "nameRole";
    roles[ScreenNameRole] = "screenNameRole";
    roles[DescriptionRole] = "descriptionRole";
    roles[AvatarRole] = "avatarRole";
    setRoleNames(roles);
}

FollowersListModel::FollowersListModel(OAuthTwitter *oauthTwitter, QObject *parent) :
    QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "nameRole";
    roles[ScreenNameRole] = "screenNameRole";
    roles[DescriptionRole] = "descriptionRole";
    roles[AvatarRole] = "avatarRole";
    setRoleNames(roles);

    m_oauthTwitter = oauthTwitter;
}

void FollowersListModel::setOAuthTwitter(OAuthTwitter *oauthTwitter)
{
    m_oauthTwitter = oauthTwitter;
}

int FollowersListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_users.count();
}

QVariant FollowersListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > m_users.count())
        return QVariant();

    const QTweetUser &user = m_users.at(index.row());

    if (role == NameRole)
        return user.name();
    else if (role == ScreenNameRole)
        return user.screenName();
    else if (role == DescriptionRole)
        return user.description();
    else if (role == AvatarRole)
        return user.profileImageUrl();

    return QVariant();
}

void FollowersListModel::fetchFollowers(const QString &cursor)
{
    if (cursor == "-1") {
        beginResetModel();
        m_users.clear();
        endResetModel();
    }

    QTweetUserStatusesFollowers *followers = new QTweetUserStatusesFollowers;
    followers->setOAuthTwitter(m_oauthTwitter);
    followers->fetch(0, cursor);
    connect(followers, SIGNAL(parsedFollowersList(QList<QTweetUser>,QString)),
            this, SLOT(followersFinished(QList<QTweetUser>,QString)));

}

void FollowersListModel::followersFinished(const QList<QTweetUser> &followers, const QString &nextCursor)
{
    QTweetUserStatusesFollowers *users = qobject_cast<QTweetUserStatusesFollowers*>(sender());

    if (users) {
        beginInsertRows(QModelIndex(), m_users.count(), m_users.count() + followers.count());
        m_users.append(followers);
        endInsertRows();

        if (nextCursor == "0")
            return;

        //continue fetchingg next page
        fetchFollowers(nextCursor);

        users->deleteLater();
    }
}
