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

#include <QtDebug>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "qtweethometimeline.h"
#include "qtweetstatus.h"
#include "qtweetconvert.h"
#include "json/qjsondocument.h"
#include "json/qjsonarray.h"

/**
 *  Constructor
 */
QTweetHomeTimeline::QTweetHomeTimeline(QObject *parent) :
    QTweetNetBase(parent),
    m_sinceid(0),
    m_maxid(0),
    m_count(0),
    m_trimUser(false),
    m_includeEntities(false),
    m_excludeReplies(false),
    m_contributorDetails(false)
{
}

/**
 *  Constructor
 *  @param oauthTwitter OAuthTwitter object
 *  @param parent parent QObject
 */
QTweetHomeTimeline::QTweetHomeTimeline(OAuthTwitter *oauthTwitter, QObject *parent) :
    QTweetNetBase(oauthTwitter, parent),
    m_sinceid(0),
    m_maxid(0),
    m_count(0),
    m_trimUser(false),
    m_includeEntities(false),
    m_excludeReplies(false),
    m_contributorDetails(false)
{
}

/**
 *   Start fetching home timeline
 *   @param sinceid fetches tweets with ID greater (more recent) then sinceid
 *   @param maxid fetches tweets with ID less (older) then maxid
 *   @param count number of tweets to fetch (up to 200)
 *   @param page page number
 *   @param skipUser true to include only status authors numerical ID
 *   @param includeEntities true to include a node called "entities"
 *   @param excludeReplies true to prevent replies from appearing in the returned timeline
 *   @param contributorDetails true to enhance the contributors element of the status response
 *   @remarks Setting parameter to default value will not be put in a querys
 */
void QTweetHomeTimeline::fetch(qint64 sinceid,
                               qint64 maxid,
                               int count,
                               bool trimUser,
                               bool includeEntities,
                               bool excludeReplies,
                               bool contributorDetails)
{
    if (!isAuthenticationEnabled()) {
        qCritical("Needs authentication to be enabled");
        return;
    }

    QUrl url("https://api.twitter.com/1.1/statuses/home_timeline.json");

    if (sinceid != 0)
        url.addQueryItem("since_id", QString::number(sinceid));

    if (maxid != 0)
        url.addQueryItem("max_id", QString::number(maxid));

    if (count != 0)
        url.addQueryItem("count", QString::number(count));

    if (trimUser)
        url.addQueryItem("trim_user", "true");

    if (includeEntities)
        url.addQueryItem("include_entities", "true");

    if (excludeReplies)
        url.addQueryItem("exclude_replies", "true");

    if (contributorDetails)
        url.addQueryItem("contributor_details", "true");

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetHomeTimeline::get()
{
    fetch(m_sinceid, m_maxid, m_count, m_trimUser, m_includeEntities, m_excludeReplies, m_contributorDetails);
}

void QTweetHomeTimeline::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isArray()) {
        QList<QTweetStatus> statuses = QTweetConvert::jsonArrayToStatusList(jsonDoc.array());

        emit parsedStatuses(statuses);
    }
}

