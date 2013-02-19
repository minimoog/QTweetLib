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
#include <QUrlQuery>
#include "qtweetdirectmessages.h"
#include "qtweetdmstatus.h"
#include "qtweetconvert.h"
#include <QJsonDocument>
#include <QJsonArray>

/**
 *  Constructor
 */
QTweetDirectMessages::QTweetDirectMessages(QObject *parent) :
    QTweetNetBase(parent)
{
}

/**
 *  Constructor
 *  @param oauthTwitter OAuthTwitter object
 *  @param parent parent QObject
 */
QTweetDirectMessages::QTweetDirectMessages(OAuthTwitter *oauthTwitter, QObject *parent) :
    QTweetNetBase(oauthTwitter, parent)
{
}

/**
 *   Start fetching direct messages
 *   @param sinceid Fetch DM newer then sinceid
 *   @param maxid Fetch DM older then maxid
 *   @param count Number of DM to fetch (up to 200)
 *   @param page Page number
 *   @param includeEntities When true each tweet will include a node called "entities"
 *   @remarks Setting parameter to default value will not be put in query
 */
void QTweetDirectMessages::fetch(qint64 sinceid,
                                 qint64 maxid,
                                 int count,
                                 int page,
                                 bool includeEntities)
{
    if (!isAuthenticationEnabled()) {
        qCritical("Needs authentication to be enabled");
        return;
    }

    QUrl url("http://api.twitter.com/1/direct_messages.json");
    QUrlQuery urlQuery;

    if (sinceid != 0)
        urlQuery.addQueryItem("since_id", QString::number(sinceid));

    if (maxid != 0)
        urlQuery.addQueryItem("max_id", QString::number(maxid));

    if (count != 0)
        urlQuery.addQueryItem("count", QString::number(count));

    if (page != 0)
        urlQuery.addQueryItem("page", QString::number(page));

    if (includeEntities)
        urlQuery.addQueryItem("include_entities", "true");

    url.setQuery(urlQuery);

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetDirectMessages::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isArray()) {
        QList<QTweetDMStatus> directMessages = QTweetConvert::jsonArrayToDirectMessagesList(jsonDoc.array());

        emit parsedDirectMessages(directMessages);
    }
}
