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
