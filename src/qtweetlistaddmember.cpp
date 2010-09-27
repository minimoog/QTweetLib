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

#include <QNetworkReply>
#include <QNetworkRequest>
#include "qtweetlistaddmember.h"
#include "qtweetlist.h"

QTweetListAddMember::QTweetListAddMember(QObject *parent) :
    QTweetNetBase(parent)
{
}

QTweetListAddMember::QTweetListAddMember(OAuthTwitter *oauthTwitter, QObject *parent) :
        QTweetNetBase(oauthTwitter, parent)
{
}

/*!
    \param user User id (owner of the list)
    \param list List id
    \param memberid User id of the list member
    \param respType Response type json or xml
 */
void QTweetListAddMember::add(qint64 user, qint64 list, qint64 memberid, ResponseType respType)
{
    Q_ASSERT(oauthTwitter() != 0);

    QString urlString = QString("http://api.twitter.com/1/%1/%2/members.").arg(user).arg(list);

    QUrl url;

    if (respType == QTweetNetBase::JSON)
        url.setUrl(urlString + "json");
    else
        url.setUrl(urlString + "xml");

    url.addQueryItem("id", QString::number(memberid));

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::POST);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->post(req, QByteArray());
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error()));
}

void QTweetListAddMember::parsingJsonFinished(const QVariant &json, bool ok, const QString &errorMsg)
{
    if (ok) {
        QTweetList list = variantMapToTweetList(json.toMap());

        emit parsedList(list);
    } else {
        qDebug() << "QTweetListAddMember parser error: " << errorMsg;
    }
}

void QTweetListAddMember::error()
{
    // ### TODO
}
