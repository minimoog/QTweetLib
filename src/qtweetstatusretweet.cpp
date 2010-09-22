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

#include <QNetworkReply>
#include <QNetworkRequest>
#include "qtweetstatusretweet.h"
#include "qtweetstatus.h"

QTweetStatusRetweet::QTweetStatusRetweet(QObject *parent) :
    QTweetNetBase(parent)
{
}

QTweetStatusRetweet::QTweetStatusRetweet(OAuthTwitter *oauthTwitter, QObject *parent) :
        QTweetNetBase(oauthTwitter, parent)
{
}

/*!
    Retweets the tweet
    \param id Tweet ID to retweet
    \param respType Response type
    \remarks Async
 */
void QTweetStatusRetweet::retweet(qint64 id,
                                  bool trimUser,
                                  bool includeEntities,
                                  ResponseType respType)
{
    Q_ASSERT(oauthTwitter() != 0);

    QUrl url;

    if (respType == QTweetNetBase::JSON)
        url.setUrl(QString("http://api.twitter.com/1/statuses/retweet/%1.json").arg(id));
    else
        url.setUrl(QString("http://api.twitter.com/1/statuses/retweet/%1.xml").arg(id));

    if (trimUser)
        url.addQueryItem("trim_user", "true");

    if (includeEntities)
        url.addQueryItem("include_entities", "true");

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::POST);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->post(req, QByteArray());
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error()));
}

void QTweetStatusRetweet::parsingJsonFinished(const QVariant &json, bool ok, const QString &errorMsg)
{
    if (ok) {
        QTweetStatus status = variantMapToStatus(json.toMap());

        emit postedRetweet(status);
    } else {
        qDebug() << "QTweetStatusRetweet JSON parser error: " << errorMsg;
    }
}

void QTweetStatusRetweet::error()
{
    // ### TODO:
}
