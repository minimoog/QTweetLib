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

#ifndef QTWEETENTITYUSERMENTIONS_H
#define QTWEETENTITYUSERMENTIONS_H

#include <QSharedDataPointer>

class QTweetEntityUserMentionsData;

class QTweetEntityUserMentions
{
public:
    QTweetEntityUserMentions();
    QTweetEntityUserMentions(const QTweetEntityUserMentions &);
    QTweetEntityUserMentions &operator=(const QTweetEntityUserMentions &);
    ~QTweetEntityUserMentions();

    void setScreenName(const QString& screenName);
    QString screenName() const;
    void setName(const QString& name);
    QString name() const;
    void setUserid(qint64 id);
    qint64 userid() const;
    void setLowerIndex(int index);
    int lowerIndex() const;
    void setHigherIndex(int index);
    int higherIndex() const;

private:
    QSharedDataPointer<QTweetEntityUserMentionsData> data;
};

#endif // QTWEETENTITYUSERMENTIONS_H
