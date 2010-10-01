/* Copyright (c) 2010, Antonie Jovanoski
 *
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact e-mail: Antonie Jovanoski <minimoog77_at_gmail.com>
 */

#include <QtDebug>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "qtweetusersearch.h"
#include "qtweetuser.h"

QTweetUserSearch::QTweetUserSearch(QObject *parent) :
    QTweetNetBase(parent)
{
}

QTweetUserSearch::QTweetUserSearch(OAuthTwitter *oauthTwitter, QObject *parent) :
    QTweetNetBase(oauthTwitter, parent)
{
}

/*!
    Starts a search
    \param query The search query to run against people search
    \param perPage The number of people per page to retrieve. Maxiumum of 20 allowed per page.
    \param page Specifies the page of results to retrieve
    \param includeEntities When set to true each tweet will include a node called "entities".
    \remarks Async
 */
void QTweetUserSearch::search(const QString &query,
                              int perPage,
                              int page,
                              bool includeEntities)
{
    Q_ASSERT(oauthTwitter() != 0);

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
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error()));
}

void QTweetUserSearch::parsingJsonFinished(const QVariant &json, bool ok, const QString &errorMsg)
{
    if (ok) {
        QList<QTweetUser> userInfoList = variantToUserInfoList(json);

        emit parsedUserInfoList(userInfoList);
    } else {
        qDebug() << "QTweetUserSearch json parser error: " << errorMsg;
    }
}

void QTweetUserSearch::error()
{
    // ### TODO
}
