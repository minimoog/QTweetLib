/* Copyright 2012 Antonie Jovanoski
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

#include "qtweetentitymedia.h"
#include <QString>
#include <QSharedData>
#include <QSize>

class QTweetEntityMediaData : public QSharedData
{
public:
    QTweetEntityMediaData() : lowerIndex(0), higherIndex(0) { }

    QString id;
    QString mediaUrl;
    QString mediaUrlHttps;
    QString url;
    QString displayUrl;
    QString expandedUrl;
    QSize largeSize;
    QSize mediumSize;
    QSize smallSize;
    QSize thumbSize;
    int lowerIndex;
    int higherIndex;
};

QTweetEntityMedia::QTweetEntityMedia()
    : data(new QTweetEntityMediaData)
{
}

QTweetEntityMedia::QTweetEntityMedia(const QTweetEntityMedia &rhs)
    : data(rhs.data)
{
}

QTweetEntityMedia& QTweetEntityMedia::operator =(const QTweetEntityMedia& rhs)
{
    if (this != &rhs)
        data.operator =(rhs.data);

    return *this;
}

QTweetEntityMedia::~QTweetEntityMedia()
{
}

void QTweetEntityMedia::setID(const QString &id)
{
    data->id = id;
}

QString QTweetEntityMedia::id() const
{
    return data->id;
}

void QTweetEntityMedia::setMediaUrl(const QString &url)
{
    data->mediaUrl = url;
}

QString QTweetEntityMedia::mediaUrl() const
{
    return data->mediaUrl;
}

void QTweetEntityMedia::setMediaUrlHttps(const QString &url)
{
    data->mediaUrlHttps = url;
}

QString QTweetEntityMedia::mediaUrlHttps() const
{
    return data->mediaUrlHttps;
}

void QTweetEntityMedia::setUrl(const QString &url)
{
    data->url = url;
}

void QTweetEntityMedia::setDisplayUrl(const QString& url)
{
    data->displayUrl = url;
}

QString QTweetEntityMedia::url() const
{
    return data->url;
}

void QTweetEntityMedia::setExpandedUrl(const QString &url)
{
    data->expandedUrl = url;
}

QString QTweetEntityMedia::expandedUrl() const
{
    return data->expandedUrl;
}

void QTweetEntityMedia::setSize(const QSize &size, MediaSize type)
{
    switch (type) {
    case LARGE:
        data->largeSize = size;
        break;

    case MEDIUM:
        data->mediumSize = size;
        break;

    case SMALL:
        data->smallSize = size;
        break;

    case THUMB:
        data->thumbSize = size;
        break;

    default:
        return;
    }
}

QSize QTweetEntityMedia::size(MediaSize type) const
{
    switch (type) {
    case LARGE:
        return data->largeSize;

    case MEDIUM:
        return data->mediumSize;

    case SMALL:
        return data->smallSize;

    case THUMB:
        return data->thumbSize;

    default:
        return QSize();
    }
}

void QTweetEntityMedia::setHigherIndex(int index)
{
    data->higherIndex = index;
}

int QTweetEntityMedia::higherIndex() const
{
    return data->higherIndex;
}

void QTweetEntityMedia::setLowerIndex(int index)
{
    data->lowerIndex = index;
}

int QTweetEntityMedia::lowerIndex() const
{
    return data->lowerIndex;
}
