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

#ifndef QTWEETACCOUNTVERIFYCREDENTIALS_H
#define QTWEETACCOUNTVERIFYCREDENTIALS_H

#include "qtweetnetbase.h"

/**
 *  Checks credentials of a authenticated user. Should emit parsedUser signal
 *  if authentication was successful, error if not. Use this to test if
 *  supplied user credentials are valid
 */
class QTWEETLIBSHARED_EXPORT QTweetAccountVerifyCredentials : public QTweetNetBase
{
    Q_OBJECT
public:
    QTweetAccountVerifyCredentials(QObject *parent = 0);
    QTweetAccountVerifyCredentials(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void verify(bool includeEntities = false);

signals:
    /** Emits parsed user when credentials are valid */
    void parsedUser(const QTweetUser& user);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};

#endif // QTWEETACCOUNTVERIFYCREDENTIALS_H
