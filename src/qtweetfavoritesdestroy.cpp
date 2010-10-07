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
#include "qtweetfavoritesdestroy.h"
#include "qtweetstatus.h"

QTweetFavoritesDestroy::QTweetFavoritesDestroy(QObject *parent) :
    QTweetNetBase(parent)
{
}

QTweetFavoritesDestroy::QTweetFavoritesDestroy(OAuthTwitter *oauthTwitter, QObject *parent) :
        QTweetNetBase(oauthTwitter, parent)
{
}

/*!
    \param statusid ID of the desired status
    \param includeEntities When set to true, each tweet will include a node called "entities,"
 */
void QTweetFavoritesDestroy::unfavorite(qint64 statusid, bool includeEntities)
{
    Q_ASSERT(oauthTwitter() != 0);

    QUrl url(QString("http://api.twitter.com/1/favorites/destroy/%1.json").arg(statusid));

    if (includeEntities)
        url.addQueryItem("include_entities", "true");

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::DELETE);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->deleteResource(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error()));
}

void QTweetFavoritesDestroy::parsingJsonFinished(const QVariant &json, bool ok, const QString &errorMsg)
{
    if (ok) {
        QTweetStatus status = variantMapToStatus(json.toMap());

        emit parsedStatus(status);
    } else {
        qDebug() << "QTweetFavoritesDestroy parser error: " << errorMsg;
    }
}

