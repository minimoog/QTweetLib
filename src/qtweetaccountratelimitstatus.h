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

#ifndef QTWEETACCOUNTRATELIMITSTATUS_H
#define QTWEETACCOUNTRATELIMITSTATUS_H

#include "qtweetnetbase.h"

/**
 *   Returns the remaining number of API requests available to the requesting user
 *   before the API limit is reached for the current hour.
 */
class QTweetAccountRateLimitStatus : public QTweetNetBase
{
    Q_OBJECT
public:
    QTweetAccountRateLimitStatus(QObject *parent = 0);
    QTweetAccountRateLimitStatus(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void check();

signals:
    /** Emits rate limit info
     *  @param remainingHits Remaining hits
     *  @param resetTime Reset time in seconds
     *  @param hourlyLimit
     */
    void rateLimitInfo(int remainingHits, int resetTime, int hourlyLimit);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};

#endif // QTWEETACCOUNTRATELIMITSTATUS_H
