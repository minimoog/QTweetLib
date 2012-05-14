/* Copyright (c) 2012, Antonie Jovanoski
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
