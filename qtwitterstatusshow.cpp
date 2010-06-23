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
#include "qtwitterstatusshow.h"

QtwitterStatusShow::QtwitterStatusShow(QObject *parent) :
    QtwitterNetBase(parent)
{
}

QtwitterStatusShow::QtwitterStatusShow(OAuthTwitter *oauthTwitter, QObject *parent) :
        QtwitterNetBase(oauthTwitter, parent)
{
}

void QtwitterStatusShow::fetch(qint64 id, ResponseType respType)
{
    Q_ASSERT(oauthTwitter() != 0);

    QUrl url;

    if (respType == QtwitterNetBase::JSON)
        url.setUrl("http://api.twitter.com/1/statuses/show.json");
    else
        url.setUrl("http://api.twitter.com/1/statuses/show.xml");

    url.addQueryItem("id", QString::number(id));

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error()));
}

void QtwitterStatusShow::reply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (reply) {
        m_response = reply->readAll();
        emit finished(m_response);

        reply->deleteLater();
    }
}

void QtwitterStatusShow::error()
{
    // ### TODO
}
