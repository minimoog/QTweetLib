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

#include "qtweetentityurl.h"
#include <QString>
#include <QSharedData>

class QTweetEntityUrlData : public QSharedData
{

public:
    QTweetEntityUrlData() : lower_index(0), higher_index(0) { }

    QString displayUrl;
    QString url;
    QString expandedUrl;
    int lower_index;
    int higher_index;
};

QTweetEntityUrl::QTweetEntityUrl()
    : data(new QTweetEntityUrlData)
{
}

QTweetEntityUrl::QTweetEntityUrl(const QTweetEntityUrl &rhs)
    : data(rhs.data)
{
}

QTweetEntityUrl &QTweetEntityUrl::operator=(const QTweetEntityUrl &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

QTweetEntityUrl::~QTweetEntityUrl()
{
}

void QTweetEntityUrl::setDisplayUrl(const QString &url)
{
    data->displayUrl = url;
}

QString QTweetEntityUrl::displayUrl() const
{
    return data->displayUrl;
}

void QTweetEntityUrl::setUrl(const QString &url)
{
    data->url = url;
}

QString QTweetEntityUrl::url() const
{
    return data->url;
}

void QTweetEntityUrl::setExpandedUrl(const QString &url)
{
    data->url = url;
}

QString QTweetEntityUrl::expandedUrl() const
{
    return data->url;
}

void QTweetEntityUrl::setLowerIndex(int index)
{
    data->lower_index = index;
}

int QTweetEntityUrl::lowerIndex() const
{
    return data->lower_index;
}

void QTweetEntityUrl::setHigherIndex(int index)
{
    data->higher_index = index;
}

int QTweetEntityUrl::higherIndex() const
{
    return data->higher_index;
}
