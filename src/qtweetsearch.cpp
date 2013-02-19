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
#include "qtweetsearch.h"
#include "qtweetsearchpageresults.h"
#include "qtweetconvert.h"
#include <QJsonDocument>
#include <QJsonObject>

QTweetSearch::QTweetSearch(QObject *parent) :
    QTweetNetBase(parent)
{
}

QTweetSearch::QTweetSearch(OAuthTwitter *oauthTwitter, QObject *parent) :
    QTweetNetBase(oauthTwitter, parent)
{
}

/**
 *  Starts searching
 *  @param query serch query
 *  @param lang Restricts tweets to the given language, given by an ISO 639-1 code.
 *  @param rpp number of tweets to return per page, up to a max of 100.
 *  @param page page number (starting at 1) to return, up to a max of roughly 1500 results
 *  @param sinceid returns results with an ID greater than (that is, more recent than) the specified ID.
 */
void QTweetSearch::start(const QString &query,
                         const QString &lang,
                         /* const QString &locale, */
                         int rpp,
                         int page,
                         qint64 sinceid)
{
    QUrl url("http://search.twitter.com/search.json");
    QUrlQuery urlQuery;

    urlQuery.addQueryItem("q", query);

    if (!lang.isEmpty())
        urlQuery.addQueryItem("lang", lang);

    // if (!locale.isEmpty())
    //     url.addQueryItem("locale", locale);

    if (rpp)
        urlQuery.addQueryItem("rpp", QString::number(rpp));

    if (page)
        urlQuery.addQueryItem("page", QString::number(page));

    if (sinceid)
        urlQuery.addQueryItem("since_id", QString::number(sinceid));

    url.setQuery(urlQuery);

    QNetworkRequest req(url);

    if (isAuthenticationEnabled()) {
        QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
        req.setRawHeader(AUTH_HEADER, oauthHeader);
    }

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetSearch::startWithCustomQuery(const QByteArray &encodedQuery)
{
    QUrl url("http://search.twitter.com/search.json");
    QUrlQuery urlQuery(encodedQuery);

    url.setQuery(urlQuery);

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetSearch::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isObject()) {
        QTweetSearchPageResults pageResults = QTweetConvert::jsonObjectToSearchPageResults(jsonDoc.object());

        emit parsedPageResults(pageResults);
    }
}
