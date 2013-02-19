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

#ifndef OAUTH_H
#define OAUTH_H

#include <QObject>
#include <QUrl>
#include "qtweetlib_global.h"

class QByteArray;

/**
 * Base OAuth class
 */
class QTWEETLIBSHARED_EXPORT OAuth : public QObject
{
    Q_OBJECT
    Q_ENUMS(HttpMethod)
    Q_PROPERTY(QByteArray oauthToken READ oauthToken WRITE setOAuthToken)
    Q_PROPERTY(QByteArray oauthTokenSecret READ oauthTokenSecret WRITE setOAuthTokenSecret)
    Q_PROPERTY(QByteArray consumerKey READ consumerKey WRITE setConsumerKey)
    Q_PROPERTY(QByteArray consumerSecret READ consumerSecret WRITE setConsumerSecret)

public:
    OAuth(QObject *parent = 0);
    OAuth(const QByteArray& consumerKey, const QByteArray& consumerSecret, QObject *parent = 0);

    enum HttpMethod {GET, POST, PUT, DELETE};

    void parseTokens(const QByteArray& response);
    QByteArray generateAuthorizationHeader(const QUrl& url, HttpMethod method);
    void setOAuthToken(const QByteArray& token);
    void setOAuthTokenSecret(const QByteArray& tokenSecret);
    void setConsumerKey(const QByteArray& key);
    void setConsumerSecret(const QByteArray& secret);
    void clearTokens();
    QByteArray oauthToken() const;
    QByteArray oauthTokenSecret() const;
    QByteArray consumerKey() const;
    QByteArray consumerSecret() const;
	
private:
    QByteArray generateSignatureHMACSHA1(const QByteArray& signatureBase);
    QByteArray generateSignatureBase(const QUrl& url, HttpMethod method, const QByteArray& timestamp, const QByteArray& nonce);

    QByteArray m_oauthToken;
    QByteArray m_oauthTokenSecret;
    QByteArray m_oauthConsumerSecret;
    QByteArray m_oauthConsumerKey;
};

#endif //OAUTH_H
