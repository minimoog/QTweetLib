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
#include "qtweetfavorites.h"
#include "qtweetstatus.h"
#include "qtweetconvert.h"
#include "json/qjsondocument.h"
#include "json/qjsonarray.h"

/**
 *  Constructor
 */
QTweetFavorites::QTweetFavorites(QObject *parent) :
    QTweetNetBase(parent)
{
}

/**
 *  Constructor
 *  @param oauthTwitter OAuthTwitter object
 *  @param parent parent QObject
 */
QTweetFavorites::QTweetFavorites(OAuthTwitter *oauthTwitter, QObject *parent) :
        QTweetNetBase(oauthTwitter, parent)
{
}

/**
 *   Starts fetching favorites
 *   @param id the ID of the user for whom to request a list of favorite statuses. 0 for authenticated user
 *   @param page Specifies the page of results to retrieve.
 *   @param includeEntities When set to true, each tweet will include a node called "entities,".
 */
void QTweetFavorites::fetch(qint64 id, int page, bool includeEntities)
{
    if (!isAuthenticationEnabled()) {
        qCritical("Needs authentication to be enabled");
        return;
    }

    QUrl url;

    if (id) {
        url.setUrl(QString("http://api.twitter.com/1/favorites/%1.json").arg(id));
    } else {
        url.setUrl("http://api.twitter.com/1/favorites.json");
    }

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

void QTweetFavorites::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isArray()) {
        QList<QTweetStatus> statuses = QTweetConvert::jsonArrayToStatusList(jsonDoc.array());

        emit parsedFavorites(statuses);
    }
}
