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

#ifndef QTWEETSTATUSSHOW_H
#define QTWEETSTATUSSHOW_H

#include "qtweetnetbase.h"

/**
 *   Class for fetching single tweet
 */
class QTWEETLIBSHARED_EXPORT QTweetStatusShow : public QTweetNetBase
{
    Q_OBJECT
    Q_PROPERTY(qint64 tweetid READ tweetid WRITE setTweetid)
    Q_PROPERTY(bool trimUser READ isTrimUser WRITE setTrimUser)
    Q_PROPERTY(bool includeMyRetweet READ includeMyRetweet WRITE setIncludeMyRetweet)
    Q_PROPERTY(bool includeEntities READ includeEntities WRITE setIncludeEntities)

public:
    QTweetStatusShow(QObject *parent = 0);
    QTweetStatusShow(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void fetch(qint64 id,
               bool trimUser = false,
               bool includeMyRetweet = false,
               bool includeEntities = false);
    void get();

    qint64 tweetid() const { return m_tweetid; }
    void setTweetid(qint64 id) { m_tweetid = id; }

    void setTrimUser(bool trimUser) { m_trimUser = trimUser; }
    bool isTrimUser() const { return m_trimUser; }

    void setIncludeMyRetweet(bool includeMyRetweet) { m_includeMyRetweet = includeMyRetweet; }
    bool includeMyRetweet() const { return m_includeMyRetweet; }

    void setIncludeEntities(bool includeEntities) { m_includeEntities = includeEntities; }
    bool includeEntities() const { return m_includeEntities; }

signals:
    /** Emits specified tweet */
    void parsedStatus(const QTweetStatus& status);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);

private:
    // ### TODO: Use pimpl
    qint64 m_tweetid;
    bool m_trimUser;
    bool m_includeMyRetweet;
    bool m_includeEntities;
};

#endif // QTWEETSTATUSSHOW_H
