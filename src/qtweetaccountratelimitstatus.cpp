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
#include "qtweetaccountratelimitstatus.h"

QTweetAccountRateLimitStatus::QTweetAccountRateLimitStatus(QObject *parent) :
    QTweetNetBase(parent)
{
}

QTweetAccountRateLimitStatus::QTweetAccountRateLimitStatus(OAuthTwitter *oauthTwitter, QObject *parent) :
        QTweetNetBase(oauthTwitter, parent)
{
}

void QTweetAccountRateLimitStatus::check()
{
    Q_ASSERT(oauthTwitter() != 0);

    QUrl url("http://api.twitter.com/1/account/rate_limit_status.json");

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error()));
}

void QTweetAccountRateLimitStatus::parsingJsonFinished(const QVariant &json, bool ok, const QString &errorMsg)
{
    if (ok) {
        QVariantMap respMap = json.toMap();

        int remainingHits = respMap["remaining_hits"].toInt();
        int resetTime = respMap["reset_time_in_seconds"].toInt();
        int hourlyLimit = respMap["hourly_limit"].toInt();

        emit rateLimitInfo(remainingHits, resetTime, hourlyLimit);
    } else {
        qDebug() << "QTweetAccountRateLimitStatus parser error: " << errorMsg;
    }
}

void QTweetAccountRateLimitStatus::error()
{
    // ### TODO
}
