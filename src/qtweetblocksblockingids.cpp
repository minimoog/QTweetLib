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

#include "qtweetblocksblockingids.h"
#include <QtDebug>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "json/qjsondocument.h"
#include "json/qjsonarray.h"

/**
 *  Constructor
 */
QTweetBlocksBlockingIDs::QTweetBlocksBlockingIDs(QObject *parent) :
    QTweetNetBase(parent)
{
}

/**
 *  Constructor
 *  @param oauthTwitter OAuthTwitter object
 *  @param parent parent QObject
 */
QTweetBlocksBlockingIDs::QTweetBlocksBlockingIDs(OAuthTwitter *oauthTwitter, QObject *parent) :
    QTweetNetBase(oauthTwitter, parent)
{
}

/**
 *  Gets the user ids which user is blocking
 */
void QTweetBlocksBlockingIDs::getIDs()
{
    if (!isAuthenticationEnabled()) {
        qCritical("Needs authentication to be enabled");
        return;
    }

    QUrl url("http://api.twitter.com/1/blocks/blocking/ids.json");

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetBlocksBlockingIDs::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isArray()) {
        QList<qint64> useridlist;

        QJsonArray varJsonArray = jsonDoc.array();

        for (int i = 0; i < varJsonArray.size(); ++i)
            useridlist.append(static_cast<qint64>(varJsonArray[i].toDouble()));

        emit finishedGettingIDs(useridlist);
    }
}
