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

#include <QtDebug>
#include <QUrl>
#include <QNetworkReply>
#include <QEventLoop>
#include <QTimer>
#include <QNetworkAccessManager>
#include "oauthtwitter.h"

#define TWITTER_REQUEST_TOKEN_URL "http://twitter.com/oauth/request_token"
#define TWITTER_ACCESS_TOKEN_URL "http://twitter.com/oauth/access_token"
#define TWITTER_AUTHORIZE_URL "http://twitter.com/oauth/authorize"
#define TWITTER_ACCESS_TOKEN_XAUTH_URL "https://api.twitter.com/oauth/access_token"

/*!
    Constructor
 */
OAuthTwitter::OAuthTwitter(QObject *parent)
    :	OAuth(parent), m_netManager(0)
{
}

/*!
    Sets network access manager
    \remarks Must be set to work properly
 */
void OAuthTwitter::setNetworkAccessManager(QNetworkAccessManager* netManager)
{
	m_netManager = netManager;
}

/*!
    Gets network access manager
 */
QNetworkAccessManager* OAuthTwitter::networkAccessManager() const
{
	return m_netManager;
}

/*!
    Gets oauth tokens using XAuth method (starts authorization process)
    \param username username
    \param password password
    \remarks Async, emits authorizeXAuthFinished or authorizeXAuthError when there is error
 */
void OAuthTwitter::authorizeXAuth(const QString &username, const QString &password)
{
    Q_ASSERT(m_netManager != 0);

    QUrl url(TWITTER_ACCESS_TOKEN_XAUTH_URL);
    url.addQueryItem("x_auth_username", username);
    url.addQueryItem("x_auth_password", password);
    url.addQueryItem("x_auth_mode", "client_auth");

    QByteArray oauthHeader = generateAuthorizationHeader(url, OAuth::POST);

    QNetworkRequest req(url);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = m_netManager->post(req, QByteArray());
    connect(reply, SIGNAL(finished()), this, SLOT(finishedAuthorization()));
}

void OAuthTwitter::finishedAuthorization()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            parseTokens(response);

            emit authorizeXAuthFinished();
        } else {
            //dump error
            qDebug() << "Network Error: " << reply->error();
            qDebug() << "Response error: " << reply->readAll();
            emit authorizeXAuthError();

        }

        reply->deleteLater();
    }
}
