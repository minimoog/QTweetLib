/* Copyright (c) 2012, Antonie Jovanoski
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

#include "qtweetstatusretweetbyid.h"
#include <QtDebug>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "qtweetconvert.h"

QTweetStatusRetweetByID::QTweetStatusRetweetByID(QObject *parent) :
    QTweetNetBase(parent),
    m_tweetid(0),
    m_count(0),
    m_page(0)
{
}

QTweetStatusRetweetByID::QTweetStatusRetweetByID(OAuthTwitter *oauthTwitter, QObject *parent) :
    QTweetNetBase(oauthTwitter, parent),
    m_tweetid(0),
    m_count(0),
    m_page(0)
{
}

void QTweetStatusRetweetByID::fetch(qint64 tweetid, int count, int page)
{
    if (!isAuthenticationEnabled()) {
        qCritical("Needs authentication to be enabled");
        return;
    }

    QString urlString = QString("https://api.twitter.com/1/statuses/%1/retweeted_by/ids.json").arg(tweetid);
    QUrl url(urlString);

    if (count != 0)
        url.addQueryItem("count", QString::number(count));

    if (page != 0)
        url.addQueryItem("page", QString::number(page));

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetStatusRetweetByID::get()
{
    fetch(m_tweetid, m_count, m_page);
}

void QTweetStatusRetweetByID::parsingJsonFinished(const QVariant &json, bool ok, const QString &errorMsg)
{
    if (ok) {
        QList<qint64> userid;
        QVariantList idList = json.toList();

        foreach (const QVariant& idVar, idList)
            userid.append(idVar.toLongLong());

        emit parsedUsersID(userid);
    } else {
        qDebug() << "QTweetStatusRetweetedByID json parser error: " << errorMsg;
        setLastErrorMessage(errorMsg);
        emit error(JsonParsingError, errorMsg);
    }
}
