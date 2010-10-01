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

#include <QNetworkRequest>
#include <QNetworkReply>
#include "qtweetdirectmessages.h"
#include "qtweetdmstatus.h"

QTweetDirectMessages::QTweetDirectMessages(QObject *parent) :
    QTweetNetBase(parent)
{
}

QTweetDirectMessages::QTweetDirectMessages(OAuthTwitter *oauthTwitter, QObject *parent) :
    QTweetNetBase(oauthTwitter, parent)
{
}

/*!
    Start fetching direct messages
    \param sinceid Fetch DM newer then sinceid
    \param maxid Fetch DM older then maxid
    \param count Number of DM to fetch (up to 200)
    \param page Page number
    \param includeEntities When true each tweet will include a node called "entities"
    \remarks Asynhronous
 */
void QTweetDirectMessages::fetch(qint64 sinceid,
                                 qint64 maxid,
                                 int count,
                                 int page,
                                 bool includeEntities)
{
    Q_ASSERT(oauthTwitter() != 0);

    QUrl url("http://api.twitter.com/1/direct_messages.json");

    if (sinceid != 0)
        url.addQueryItem("since_id", QString::number(sinceid));

    if (maxid != 0)
        url.addQueryItem("max_id", QString::number(maxid));

    if (count != 0)
        url.addQueryItem("count", QString::number(count));

    if (page != 0)
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

void QTweetDirectMessages::parsingJsonFinished(const QVariant &json, bool ok, const QString &errorMsg)
{
    if (ok) {
        QList<QTweetDMStatus> directMessages = variantToDirectMessagesList(json);

        emit parsedDirectMessages(directMessages);
    } else {
        qDebug() << "QTweetDirectMessages JSON parser error: " << errorMsg;
    }
}

void QTweetDirectMessages::error()
{
    // ### TODO: Better error detection
    qCritical("Direct Messages error");
}
