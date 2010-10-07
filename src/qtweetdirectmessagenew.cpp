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

#include <QNetworkRequest>
#include <QNetworkReply>
#include "qtweetdirectmessagenew.h"
#include "qtweetdmstatus.h"

QTweetDirectMessageNew::QTweetDirectMessageNew(QObject *parent) :
    QTweetNetBase(parent)
{
}

QTweetDirectMessageNew::QTweetDirectMessageNew(OAuthTwitter *oauthTwitter, QObject *parent) :
        QTweetNetBase(oauthTwitter, parent)
{
}

/*!
    \param user The ID of the user who should receive the direct message.
    \param text The text of direct message
    \param screenName The screen name of the user who should receive the direct message.
    \param includeEntities When set to true each tweet will include a node called "entities,"
 */
void QTweetDirectMessageNew::post(qint64 user,
                                  const QString &text,
                                  const QString &screenName,
                                  bool includeEntities)
{
    Q_ASSERT(oauthTwitter() != 0);

    QUrl url("http://api.twitter.com/1/direct_messages/new.json");

    if (user)
        url.addQueryItem("user_id", QString::number(user));

    url.addEncodedQueryItem("text", QUrl::toPercentEncoding(text));

    if (!screenName.isEmpty())
        url.addQueryItem("screen_name", screenName);

    if (includeEntities)
        url.addQueryItem("include_entities", "true");

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::POST);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->post(req, QByteArray());
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error()));
}

void QTweetDirectMessageNew::parsingJsonFinished(const QVariant &json, bool ok, const QString &errorMsg)
{
    if (ok) {
        QTweetDMStatus dm = variantMapToDirectMessage(json.toMap());

        emit parsedDirectMessage(dm);
    } else {
        qDebug() << "QTweetDirectMessageNew parser error: " << errorMsg;
    }
}

void QTweetDirectMessageNew::error()
{
    // ### TODO
}
