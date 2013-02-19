/* Copyright 2010 Antonie Jovanoski
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Contact e-mail: Antonie Jovanoski <minimoog77_at_gmail.com>
 */

#include <QtDebug>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "qtweetaccountratelimitstatus.h"
#include "json/qjsondocument.h"
#include "json/qjsonobject.h"

/**
 *  Constructor
 */
QTweetAccountRateLimitStatus::QTweetAccountRateLimitStatus(QObject *parent) :
    QTweetNetBase(parent)
{
}

/**
 *  Constructor
 *  @param oauthTwitter OAuthTwitter object
 *  @param parent parent QObject
 */
QTweetAccountRateLimitStatus::QTweetAccountRateLimitStatus(OAuthTwitter *oauthTwitter, QObject *parent) :
        QTweetNetBase(oauthTwitter, parent)
{
}

/**
 *  Starts checking rate limit status
 *  @remarks Should be emiting rateLimitInfo signal after finishing
 */
void QTweetAccountRateLimitStatus::check()
{
    QUrl url("http://api.twitter.com/1/account/rate_limit_status.json");

    QNetworkRequest req(url);

    if (isAuthenticationEnabled()) {
        QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
        req.setRawHeader(AUTH_HEADER, oauthHeader);
    }

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetAccountRateLimitStatus::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isObject()) {
        QJsonObject respJsonObject = jsonDoc.object();

        int remainingHits = static_cast<int>(respJsonObject["remaining_hits"].toDouble());
        int resetTime = static_cast<int>(respJsonObject["reset_time_in_seconds"].toDouble());
        int hourlyLimit = static_cast<int>(respJsonObject["hourly_limit"].toDouble());

        emit rateLimitInfo(remainingHits, resetTime, hourlyLimit);
    }
}
