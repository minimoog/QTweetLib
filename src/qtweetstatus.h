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

#ifndef QTWEETSTATUS_H
#define QTWEETSTATUS_H

#include <QVariant>
#include <QSharedDataPointer>
#include "qtweetlib_global.h"

class QDateTime;
class QTweetUser;
class QTweetStatusData;
class QTweetPlace;
class QTweetEntityUrl;
class QTweetEntityHashtag;
class QTweetEntityUserMentions;
class QTweetEntityMedia;

/**
 *   Stores tweet info
 */
class QTWEETLIBSHARED_EXPORT QTweetStatus
{
public:
    QTweetStatus();
    QTweetStatus(const QTweetStatus &other);
    QTweetStatus &operator=(const QTweetStatus &other);
    ~QTweetStatus();

    void setId(qint64 id);
    qint64 id() const;
    void setText(const QString& text);
    QString text() const;
    void setCreatedAt(const QString& twitterDate);
    void setCreatedAt(const QDateTime& dateTime);
    QDateTime createdAt() const;
    void setInReplyToUserId(qint64 id);
    qint64 inReplyToUserId() const;
    void setInReplyToScreenName(const QString& screenName);
    QString inReplyToScreenName() const;
    void setInReplyToStatusId(qint64 id);
    qint64 inReplyToStatusId() const;
    void setFavorited(bool fav);
    bool favorited() const;
    void setSource(const QString& source);
    QString source() const;
    void setUser(const QTweetUser& user);
    QTweetUser user() const;
    qint64 userid() const;  //usefull for getting quick userid when userinfo is trimmed
    void setRetweetedStatus(const QTweetStatus& status);
    QTweetStatus retweetedStatus() const;
    void setPlace(const QTweetPlace& place);
    QTweetPlace place() const;
    bool isRetweet() const;
    QList<QTweetEntityUrl> urlEntities() const;
    QList<QTweetEntityHashtag> hashtagEntities() const;
    QList<QTweetEntityUserMentions> userMentionsEntities() const;
    QList<QTweetEntityMedia> mediaEntities() const;
    void addUrlEntity(const QTweetEntityUrl& urlEntity);
    void addHashtagEntity(const QTweetEntityHashtag& hashtagEntity);
    void addUserMentionsEntity(const QTweetEntityUserMentions& userMentionsEntity);
    void addMediaEntity(const QTweetEntityMedia& mediaEntity);

private:
    QSharedDataPointer<QTweetStatusData> d;
};

Q_DECLARE_METATYPE(QTweetStatus)

#endif // QTWEETSTATUS_H
