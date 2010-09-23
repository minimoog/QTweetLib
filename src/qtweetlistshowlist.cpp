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
#include "qtweetlistshowlist.h"
#include "qtweetlist.h"

QTweetListShowList::QTweetListShowList(QObject *parent) :
    QTweetNetBase(parent)
{
}

QTweetListShowList::QTweetListShowList(OAuthTwitter *oauthTwitter, QObject *parent) :
        QTweetNetBase(oauthTwitter, parent)
{
}

/*!
    Shows (gets) specified list
    \param id User id
    \param list List id
    \param respType Response type json or xml
 */
void QTweetListShowList::show(qint64 id, qint64 list, ResponseType respType)
{
    // slug parameter?

    Q_ASSERT(oauthTwitter() != 0);

    QString urlString = QString("http://api.twitter.com/1/%1/lists/%2.").arg(id).arg(list);

    QUrl url;

    if (respType == QTweetNetBase::JSON)
        url.setUrl(urlString + "json");
    else
        url.setUrl(urlString + "xml");

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error()));
}

void QTweetListShowList::parsingJsonFinished(const QVariant &json, bool ok, const QString &errorMsg)
{
    if (ok) {
        QTweetList list = variantMapToTweetList(json.toMap());

        emit parsedList(list);
    } else {
        qDebug() << "QTweetListShowList json parser error: " << errorMsg;
    }
}

void QTweetListShowList::error()
{
    // ### TODO
}
