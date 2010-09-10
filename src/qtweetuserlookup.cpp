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
#include "qtweetuserlookup.h"

QTweetUserLookup::QTweetUserLookup(QObject *parent) :
    QTweetNetBase(parent)
{
}

QTweetUserLookup::QTweetUserLookup(OAuthTwitter *oauthTwitter, QObject *parent) :
        QTweetNetBase(oauthTwitter, parent)
{
}

/*!
    Startw fetching
    \param useridList List of user IDs
    \param screenNameLit List of screen names
    \param respType Response type
    \remarks Async
 */
void QTweetUserLookup::fetch(const QList<qint64> &useridList,
                             const QStringList &screenNameList,
                             ResponseType respType)
{
    Q_ASSERT(oauthTwitter() != 0);

    QUrl url;

    if (respType == QTweetNetBase::JSON)
        url.setUrl("http://api.twitter.com/1/users/lookup.json");
    else
        url.setUrl("http://api.twitter.com/1/users/lookup.xml");

    if (!useridList.isEmpty()) {
        QString idString;

        foreach (qint64 id, useridList)
            idString += QString::number(id) + ',';

        idString.chop(1);

        url.addQueryItem("user_id", idString);
    }

    if (!screenNameList.isEmpty()) {
        QString scrnString;

        foreach (const QString& screenName, screenNameList)
            scrnString += screenName + ',';

        scrnString.chop(1);

        url.addQueryItem("screen_name", scrnString);
    }

    // ### TODO: Add include_entities parameter

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error()));
}

void QTweetUserLookup::reply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (reply) {
         m_response = reply->readAll();
        emit finished(m_response);

        reply->deleteLater();
    }
}

void QTweetUserLookup::error()
{
    // ### TODO:
}
