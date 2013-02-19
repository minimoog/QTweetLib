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

#include "qtweetentityhashtag.h"
#include <QString>
#include <QSharedData>

class QTweetEntityHashtagData : public QSharedData
{
public:
    QString hashtag;
    int lower_index;
    int higher_index;
};

QTweetEntityHashtag::QTweetEntityHashtag()
    : data(new QTweetEntityHashtagData)
{
}

QTweetEntityHashtag::QTweetEntityHashtag(const QTweetEntityHashtag &rhs)
    : data(rhs.data)
{
}

QTweetEntityHashtag &QTweetEntityHashtag::operator=(const QTweetEntityHashtag &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

QTweetEntityHashtag::~QTweetEntityHashtag()
{
}

void QTweetEntityHashtag::setText(const QString &text)
{
    data->hashtag = text;
}

QString QTweetEntityHashtag::text() const
{
    return data->hashtag;
}

void QTweetEntityHashtag::setLowerIndex(int index)
{
    data->lower_index = index;
}

int QTweetEntityHashtag::lowerIndex() const
{
    return data->lower_index;
}

void QTweetEntityHashtag::setHigherIndex(int index)
{
    data->higher_index = index;
}

int QTweetEntityHashtag::higherIndex() const
{
    return data->higher_index;
}
