/* Copyright 2011 Antonie Jovanoski
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

#include "qtweetblocksblocking.h"
#include <QtDebug>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "qtweetuser.h"
#include "qtweetconvert.h"
#include "json/qjsondocument.h"
#include "json/qjsonarray.h"

/**
 *  Constructor
 */
QTweetBlocksBlocking::QTweetBlocksBlocking(QObject *parent) :
    QTweetNetBase(parent)
{
}

/**
 *  Constructor
 *  @param oauthTwitter OAuthTwitter object
 *  @param parent parent QObject
 */
QTweetBlocksBlocking::QTweetBlocksBlocking(OAuthTwitter *oauthTwitter, QObject *parent) :
    QTweetNetBase(oauthTwitter, parent)
{
}

/**
 *  Gets the list of blocks
 *  @param page specifies the page of results to retrieve
 *  @param includeEntities When set to true, each tweet will include a node called "entities"
 */
void QTweetBlocksBlocking::getBlocks(int page, bool includeEntities)
{
    if (!isAuthenticationEnabled()) {
        qCritical("Needs authentication to be enabled");
        return;
    }

    QUrl url("http://api.twitter.com/1/blocks/blocking.json");

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

void QTweetBlocksBlocking::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isArray()) {
        QList<QTweetUser> userlist = QTweetConvert::jsonArrayToUserInfoList(jsonDoc.array());

        emit finishedGettingBlocks(userlist);
    }
}
