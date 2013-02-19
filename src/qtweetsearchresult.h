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

#ifndef QTWEETSEARCHRESULT_H
#define QTWEETSEARCHRESULT_H

#include <QVariant>
#include <QSharedDataPointer>
#include "qtweetlib_global.h"

class QTweetSearchResultData;

/**
 *  Stores search result
 */
class QTWEETLIBSHARED_EXPORT QTweetSearchResult
{
public:
    QTweetSearchResult();
    QTweetSearchResult(const QTweetSearchResult &);
    QTweetSearchResult &operator=(const QTweetSearchResult &);
    ~QTweetSearchResult();

    void setCreatedAt(const QString& twitterDate);
    void setCreatedAt(const QDateTime& dateTime);
    QDateTime createdAt() const;
    void setFromUser(const QString& screenName);
    QString fromUser() const;
    void setId(qint64 id);
    qint64 id() const;
    void setLang(const QString& lang);
    QString lang() const;
    void setProfileImageUrl(const QString& url);
    QString profileImageUrl() const;
    void setSource(const QString& source);
    QString source() const;
    void setText(const QString& text);
    QString text() const;
    void setToUser(const QString& screenName);
    QString toUser() const;

private:
    QSharedDataPointer<QTweetSearchResultData> data;
};

#endif // QTWEETSEARCHRESULT_H
