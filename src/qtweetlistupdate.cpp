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
#include "qtweetlistupdate.h"
#include "qtweetlist.h"

QTweetListUpdate::QTweetListUpdate(QObject *parent) :
        QTweetNetBase(parent)
{
}

QTweetListUpdate::QTweetListUpdate(OAuthTwitter *oauthTwitter, QObject *parent) :
        QTweetNetBase(oauthTwitter, parent)
{
}

/*! Updates the list
    \param user User id
    \param list List id
    \param name The name of the list
    \param mode True for public, false for private
    \param description The description to give the list
    \param respType Response type xml or json
 */
void QTweetListUpdate::update(qint64 user,
                              qint64 list,
                              const QString &name,
                              bool mode,
                              const QString &description,
                              ResponseType respType)
{
    Q_ASSERT(oauthTwitter() != 0);

    QString urlString = QString("http://api.twitter.com/1/%1/lists/%2.").arg(user).arg(list);

    QUrl url;

    if (respType == QTweetNetBase::JSON)
        url.setUrl(urlString + "json");
    else
        url.setUrl(urlString + "xml");

    if (!name.isEmpty())
        url.addQueryItem("name", name);

    if (!mode)
        url.addQueryItem("mode", "private");

    if (!description.isEmpty())
        url.addQueryItem("description", description);

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::POST);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->post(req, QByteArray());
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error()));
}

void QTweetListUpdate::parsingJsonFinished(const QVariant &json, bool ok, const QString &errorMsg)
{
    if (ok) {
        QTweetList list = variantMapToTweetList(json.toMap());

        emit parsedList(list);
    } else {
        qDebug() << "QTweetListUpdate json parser error: " << errorMsg;
    }
}

void QTweetListUpdate::error()
{
    // ### TODO
}
