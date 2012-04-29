/* Copyright (c) 2011, Antonie Jovanoski
 *
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact e-mail: Antonie Jovanoski <minimoog77_at_gmail.com>
 */

#ifndef QTWEETPUBLICTIMELINE_H
#define QTWEETPUBLICTIMELINE_H

#include "qtweetnetbase.h"

class QTWEETLIBSHARED_EXPORT QTweetPublicTimeline : public QTweetNetBase
{
    Q_OBJECT
    Q_PROPERTY(bool trimUser READ isTrimUser WRITE setTrimUser)
    Q_PROPERTY(bool includeEntities READ isIncludeEntities WRITE setIncludeEntities)

public:
    QTweetPublicTimeline(QObject *parent = 0);
    QTweetPublicTimeline(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void fetch(bool trimUser = false, bool includeEntities = false);
    void get();

    void setTrimUser(bool trimUser) { m_trimUser =  trimUser; }
    bool isTrimUser() const { return m_trimUser; }

    void setIncludeEntities(bool includeEntities) { m_includeEntities = includeEntities; }
    bool isIncludeEntities() const { return m_includeEntities; }

signals:
    void parsedStatuses(const QList<QTweetStatus>& statuses);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);

private:
    // ### TODO: Pimpl
    bool m_trimUser;
    bool m_includeEntities;
};

#endif // QTWEETPUBLICTIMELINE_H
