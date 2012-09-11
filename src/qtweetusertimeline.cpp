/* Copyright (c) 2010, Antonie Jovanoski
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
#include "qtweetusertimeline.h"
#include "qtweetstatus.h"
#include "qtweetconvert.h"
#include "json/qjsondocument.h"
#include "json/qjsonarray.h"

QTweetUserTimeline::QTweetUserTimeline(QObject *parent) :
    QTweetNetBase(parent),
    m_userid(0),
    m_sinceid(0),
    m_maxid(0),
    m_count(0),
    m_page(0),
    m_trimUser(false),
    m_includeRts(false),
    m_includeEntities(false),
    m_excludeReplies(false),
    m_contributorDetails(false)
{
}

QTweetUserTimeline::QTweetUserTimeline(OAuthTwitter *oauthTwitter, QObject *parent) :
    QTweetNetBase(oauthTwitter, parent),
    m_userid(0),
    m_sinceid(0),
    m_maxid(0),
    m_count(0),
    m_page(0),
    m_trimUser(false),
    m_includeRts(false),
    m_includeEntities(false),
    m_excludeReplies(false),
    m_contributorDetails(false)
{
}

/**
 *   Starts fetching
 *   @param userid user ID
 *   @param screenName user screen name
 *   @param sinceid fetches tweets with ID greater (more recent) then sinceid
 *   @param maxid fetches tweets with ID less (older) then maxid
 *   @param count number of tweets to fetch (up to 200)
 *   @param page page number
 *   @param skipUser true to include only status authors numerical ID
 *   @param includeRts timeline contains native retweets if true
 *   @param includeEntities each tweet include node "entities"
 *   @param excludeReplies true to prevent replies from appearing in the returned timeline
 *   @param contributorDetails true to enhance the contributors element of the status response
 */
void QTweetUserTimeline::fetch(qint64 userid,
                               const QString &screenName,
                               qint64 sinceid,
                               qint64 maxid,
                               int count,
                               int page,
                               bool trimUser,
                               bool includeRts,
                               bool includeEntities,
                               bool excludeReplies,
                               bool contributorDetails)
{
    QUrl url("https://api.twitter.com/1.1/statuses/user_timeline.json");

    if (userid != 0)
        url.addQueryItem("user_id", QString::number(userid));

    if (!screenName.isEmpty())
        url.addQueryItem("screen_name", screenName);

    if (sinceid != 0)
        url.addQueryItem("since_id", QString::number(sinceid));

    if (maxid != 0)
        url.addQueryItem("max_id", QString::number(maxid));

    if (count != 0)
        url.addQueryItem("count", QString::number(count));

    if (page != 0)
        url.addQueryItem("page", QString::number(page));

    if (trimUser)
        url.addQueryItem("trim_user", "true");

    if (includeRts)
        url.addQueryItem("include_rts", "true");

    if (includeEntities)
        url.addQueryItem("include_entities", "true");

    if (excludeReplies)
        url.addQueryItem("exclude_replies", "true");

    if (contributorDetails)
        url.addQueryItem("contributor_details", "true");

    QNetworkRequest req(url);

    if (isAuthenticationEnabled()) {
        QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
        req.setRawHeader(AUTH_HEADER, oauthHeader);
    }

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetUserTimeline::get()
{
    fetch(m_userid,
          m_screenName,
          m_sinceid,
          m_maxid,
          m_count,
          m_page,
          m_trimUser,
          m_includeRts,
          m_includeEntities,
          m_excludeReplies,
          m_contributorDetails);
}

void QTweetUserTimeline::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isArray()) {
        QList<QTweetStatus> statuses = QTweetConvert::jsonArrayToStatusList(jsonDoc.array());

        emit parsedStatuses(statuses);
    }
}
