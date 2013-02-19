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
#include "qtweetusersearch.h"
#include "qtweetuser.h"
#include "qtweetconvert.h"
#include "json/qjsondocument.h"
#include "json/qjsonarray.h"

QTweetUserSearch::QTweetUserSearch(QObject *parent) :
    QTweetNetBase(parent)
{
}

QTweetUserSearch::QTweetUserSearch(OAuthTwitter *oauthTwitter, QObject *parent) :
    QTweetNetBase(oauthTwitter, parent)
{
}

/**
 *   Starts a search
 *   @param query the search query to run against people search
 *   @param perPage the number of people per page to retrieve. Maxiumum of 20 allowed per page.
 *   @param page specifies the page of results to retrieve
 *   @param includeEntities when set to true each tweet will include a node called "entities".
 */
void QTweetUserSearch::search(const QString &query,
                              int perPage,
                              int page,
                              bool includeEntities)
{
    if (!isAuthenticationEnabled()) {
        qCritical("Needs authentication to be enabled");
        return;
    }

    QUrl url("http://api.twitter.com/1/users/search.json");

    url.addQueryItem("q", query);

    if (perPage)
        url.addQueryItem("per_page", QString::number(perPage));

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

void QTweetUserSearch::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isArray()) {
        QList<QTweetUser> userInfoList = QTweetConvert::jsonArrayToUserInfoList(jsonDoc.array());

        emit parsedUserInfoList(userInfoList);
    }
}

