/* Copyright (c) 2010, Antonie Jovanoski
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
    void authorizeXAuth(const QString& username, const QString& password);
    void authorizePin();

signals:
    /** Emited when XAuth authorization is finished */
    void authorizeXAuthFinished();
    /** Emited when pin authorization is finished. This is where user should enter PIN number */
    void authorizePinFinished();
    /** Emited when access token is taken after entering PIN. This means that we can use Twitter services */
    void accessTokenGranted();
    /** Emited when there is error in XAuth authorization */
    // ### TODO Error detection
    void authorizeXAuthError();

public slots:
    void requestAccessToken(const QString& pin);

protected:
    virtual void requestAuthorization();

private slots:
    void finishedAuthorization();

private:
    QNetworkAccessManager *m_netManager;
};	

#endif //OAUTHTWITTER_H
