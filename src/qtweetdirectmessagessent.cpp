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
#include "qtweetdirectmessagessent.h"
#include "qtweetdmstatus.h"
#include "qtweetconvert.h"
#include "json/qjsondocument.h"
#include "json/qjsonarray.h"

/**
 *  Constructor
 */
QTweetDirectMessagesSent::QTweetDirectMessagesSent(QObject *parent) :
        QTweetNetBase(parent)
{
}

/**
 *  Constructor
 *  @param oauthTwitter OAuthTwitter object
 *  @param parent parent QObject
 */
QTweetDirectMessagesSent::QTweetDirectMessagesSent(OAuthTwitter *oauthTwitter, QObject *parent) :
        QTweetNetBase(oauthTwitter, parent)
{
}

/**
 *   Starts fetching direct messages
 *   @param sinceid returns results with an ID greater than (that is, more recent than) the specified ID.
 *   @param maxid returns results with an ID less than (that is, older than) or equal to the specified ID.
 *   @param count specifies the number of records to retrieve. Must be less than or equal to 200.
 *   @param page specifies the page of results to retrieve.
 *   @param includeEntities When set to true, each tweet will include a node called "entities,".
 *   @remarks Setting parameters to default value will not be put in the query
 */
void QTweetDirectMessagesSent::fetch(qint64 sinceid,
                                     qint64 maxid,
                                     int count,
                                     int page,
                                     bool includeEntities)
{
    if (!isAuthenticationEnabled()) {
        qCritical("Needs authentication to be enabled");
        return;
    }

    QUrl url("https://api.twitter.com/1.1/direct_messages/sent.json");

    if (sinceid)
        url.addQueryItem("since_id", QString::number(sinceid));

    if (maxid)
        url.addQueryItem("max_id", QString::number(maxid));

    if (count)
        url.addQueryItem("count", QString::number(count));

    if (page)
        url.addQueryItem("page", QString::number(page));

    if (includeEntities)
        url.addQueryItem("include_entities", "true");

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetDirectMessagesSent::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isArray()) {
        QList<QTweetDMStatus> directMessages = QTweetConvert::jsonArrayToDirectMessagesList(jsonDoc.array());

        emit parsedDirectMessages(directMessages);
    }
}

