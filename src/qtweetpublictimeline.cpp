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

#include <QtDebug>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "qtweetpublictimeline.h"
#include "qtweetstatus.h"
#include "qtweetconvert.h"

QTweetPublicTimeline::QTweetPublicTimeline(QObject *parent) :
    QTweetNetBase(parent),
    m_trimUser(false),
    m_includeEntities(false)
{
}

QTweetPublicTimeline::QTweetPublicTimeline(OAuthTwitter *oauthTwitter, QObject *parent) :
    QTweetNetBase(oauthTwitter, parent),
    m_trimUser(false),
    m_includeEntities(false)
{
}

void QTweetPublicTimeline::fetch(bool trimUser, bool includeEntities)
{
    QUrl url("https://api.twitter.com/1/statuses/public_timeline.json");

    if (trimUser)
        url.addQueryItem("trim_user", "true");

    if (includeEntities)
        url.addQueryItem("include_entities", "true");

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetPublicTimeline::get()
{
    fetch(m_trimUser, m_includeEntities);
}

void QTweetPublicTimeline::parseJsonFinished(cJSON *root)
{
    QList<QTweetStatus> statuses = QTweetConvert::cJSONToStatusList(root);

    emit parsedStatuses(statuses);
}
