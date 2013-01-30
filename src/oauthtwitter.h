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

#ifndef OAUTHTWITTER_H
#define OAUTHTWITTER_H

#include "oauth.h"

class QNetworkAccessManager;

/**
 *   OAuth Twitter authorization class
 */
class QTWEETLIBSHARED_EXPORT OAuthTwitter : public OAuth
{
    Q_OBJECT
    Q_PROPERTY(QNetworkAccessManager* networkAccessManager
               READ networkAccessManager
               WRITE setNetworkAccessManager)
public:
    OAuthTwitter(QObject *parent = 0);
    OAuthTwitter(QNetworkAccessManager* netManager, QObject *parent = 0);
    OAuthTwitter(const QByteArray& consumerKey, const QByteArray& consumerSecret, QObject *parent = 0);
    void setNetworkAccessManager(QNetworkAccessManager* netManager);
    QNetworkAccessManager* networkAccessManager() const;

signals:
    /** Emited when XAuth authorization is finished */
    void authorizeXAuthFinished();
    /** Emited when pin authenticate is finished. This is where user should enter PIN number */
    void authorizePinAuthenticate();
    /** Emited when PIN authorization is finished (access tokens granted) */
    void authorizePinFinished();
    /** Emited when there is error in XAuth authorization */
    // ### TODO Error detection
    void authorizeXAuthError();

public slots:
    void requestAccessToken(const QString& pin);
    void authorizeXAuth(const QString& username, const QString& password);
    void authorizePin();

protected:
    virtual void requestAuthorization();

private slots:
    void finishedAuthorization();

private:
    QNetworkAccessManager *m_netManager;
};	

#endif //OAUTHTWITTER_H
