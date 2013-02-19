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

#include "qtweetentityusermentions.h"
#include <QSharedData>
#include <QString>

class QTweetEntityUserMentionsData : public QSharedData
{
public:
    QTweetEntityUserMentionsData() : userid(0) {}

    QString screenName;
    QString name;
    qint64 userid;
    int lower_index;
    int higher_index;
};

QTweetEntityUserMentions::QTweetEntityUserMentions()
    : data(new QTweetEntityUserMentionsData)
{
}

QTweetEntityUserMentions::QTweetEntityUserMentions(const QTweetEntityUserMentions &rhs)
    : data(rhs.data)
{
}

QTweetEntityUserMentions &QTweetEntityUserMentions::operator=(const QTweetEntityUserMentions &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

QTweetEntityUserMentions::~QTweetEntityUserMentions()
{
}

void QTweetEntityUserMentions::setScreenName(const QString &screenName)
{
    data->screenName = screenName;
}

QString QTweetEntityUserMentions::screenName() const
{
    return data->screenName;
}

void QTweetEntityUserMentions::setName(const QString &name)
{
    data->name = name;
}

QString QTweetEntityUserMentions::name() const
{
    return data->name;
}

void QTweetEntityUserMentions::setUserid(qint64 id)
{
    data->userid = id;
}

qint64 QTweetEntityUserMentions::userid() const
{
    return data->userid;
}

void QTweetEntityUserMentions::setLowerIndex(int index)
{
    data->lower_index = index;
}

int QTweetEntityUserMentions::lowerIndex() const
{
    return data->lower_index;
}

void QTweetEntityUserMentions::setHigherIndex(int index)
{
    data->higher_index = index;
}

int QTweetEntityUserMentions::higherIndex() const
{
    return data->higher_index;
}
