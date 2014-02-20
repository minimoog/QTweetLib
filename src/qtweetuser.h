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

#ifndef QTWEETUSER_H
#define QTWEETUSER_H

#include <QVariant>
#include <QSharedDataPointer>
#include "qtweetlib_global.h"

class QTweetStatus;
class QTweetUserData;

/**
 *   Class for storing user info
 */
class QTWEETLIBSHARED_EXPORT QTweetUser
{
public:
    QTweetUser();
    QTweetUser(const QTweetUser& other);
    QTweetUser& operator=(const QTweetUser& other);
    ~QTweetUser();

    void setContributorsEnabled(bool enabled);
    bool isContributorsEnabled() const;
    void setId(qint64 id);
    qint64 id() const;
    void setLang(const QString& lang);
    QString lang() const;
    void setListedCount(int count);
    int listedCount() const;
    void setName(const QString& name);
    QString name() const;
    void setScreenName(const QString& screenName);
    QString screenName() const;
    void setLocation(const QString& location);
    QString location() const;
    void setDescription(const QString& desc);
    QString description() const;
    void setprofileImageUrl(const QString& url);
    QString profileImageUrl() const;
    void setUrl(const QString& url);
    QString url() const;
    void setProtected(bool isProtected);
    bool isProtected() const;
    void setFollowersCount(int count);
    int followersCount() const;
    void setFriendsCount(int count);
    int friendsCount() const;
    void setCreatedAt(const QString& twitterDate);
    void setCreatedAt(const QDateTime& datetime);
    QDateTime createdAt() const;
    void setFavouritesCount(int count);
    int favouritesCount() const;
    void setUtcOffset(int sec);
    int utcOffset() const;
    void setTimezone(const QString& timezone);
    QString timezone() const;
    void setGeoEnabled(bool isGeoEnabled);
    bool isGeoEnabled() const;
    void setVerified(bool verified);
    bool isVerified() const;
    void setStatusesCount(int count);
    int statusesCount() const;
    void setFollowing(bool following);
    bool getFollowing() const;
    void setStatus(const QTweetStatus& lastStatus);
    QTweetStatus status() const;

    static QDateTime twitterDateToQDateTime(const QString& twitterDate);

private:
    QSharedDataPointer<QTweetUserData> d;
};

Q_DECLARE_METATYPE(QTweetUser)

#endif // QTWEETUSER_H
