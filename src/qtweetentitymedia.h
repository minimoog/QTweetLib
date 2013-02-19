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

#ifndef QTWEETENTITYMEDIA_H
#define QTWEETENTITYMEDIA_H

#include <QSharedDataPointer>

class QTweetEntityMediaData;
class QSize;

class QTweetEntityMedia
{
public:
    QTweetEntityMedia();
    QTweetEntityMedia(const QTweetEntityMedia& );
    QTweetEntityMedia & operator = (const QTweetEntityMedia& );
    ~QTweetEntityMedia();

    enum MediaSize {
        LARGE,
        MEDIUM,
        SMALL,
        THUMB
    };

    void setID(const QString& id);
    QString id() const;

    void setMediaUrl(const QString& url);
    QString mediaUrl() const;

    void setMediaUrlHttps(const QString& url);
    QString mediaUrlHttps() const;

    void setUrl(const QString& url);
    QString url() const;

    void setDisplayUrl(const QString& url);
    QString  displayUrl() const;

    void setExpandedUrl(const QString& url);
    QString expandedUrl() const;

    void setSize(const QSize& size, MediaSize type);
    QSize size(MediaSize type) const;

    void setHigherIndex(int index);
    int higherIndex() const;

    void setLowerIndex(int index);
    int lowerIndex() const;

private:
    QSharedDataPointer<QTweetEntityMediaData> data;
};

#endif // QTWEETENTITYMEDIA_H
