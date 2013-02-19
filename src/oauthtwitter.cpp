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

#include "oauthtwitter.h"
#include <QtDebug>
#include <QUrl>
#include <QNetworkReply>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QDesktopServices>

#define TWITTER_REQUEST_TOKEN_URL "https://twitter.com/oauth/request_token"
#define TWITTER_ACCESS_TOKEN_URL "https://twitter.com/oauth/access_token"
#define TWITTER_AUTHORIZE_URL "https://twitter.com/oauth/authorize"
#define TWITTER_ACCESS_TOKEN_XAUTH_URL "https://api.twitter.com/oauth/access_token"

/**
 *   Constructor
 */
OAuthTwitter::OAuthTwitter(QObject *parent)
    :	OAuth(parent), m_netManager(0)
{
}

/**
 *  Constructor
 */
OAuthTwitter::OAuthTwitter(QNetworkAccessManager *netManager, QObject *parent) :
    OAuth(parent), m_netManager(netManager)
{
}

/**
 *  Constructor
 *  @param consumerKey OAuth consumer key
 *  @param consumerSecret OAuth consumer secret
 *  @param parent parent object
 */
OAuthTwitter::OAuthTwitter(const QByteArray &consumerKey, const QByteArray &consumerSecret, QObject *parent) :
    OAuth(consumerKey, consumerSecret, parent), m_netManager(0)
{
}

/**
 *   Sets network access manager
 *   @remarks Must be set to work properly
 */
void OAuthTwitter::setNetworkAccessManager(QNetworkAccessManager* netManager)
{
	m_netManager = netManager;
}

/**
 *   Gets network access manager
 */
QNetworkAccessManager* OAuthTwitter::networkAccessManager() const
{
	return m_netManager;
}

/**
 *   Gets oauth tokens using XAuth method (starts authorization process)
 *   @param username username
 *   @param password password
 *   @remarks Async, emits authorizeXAuthFinished or authorizeXAuthError when there is error
 */
void OAuthTwitter::authorizeXAuth(const QString &username, const QString &password)
{
    Q_ASSERT(m_netManager != 0);

    QUrl url(TWITTER_ACCESS_TOKEN_XAUTH_URL);
    url.addEncodedQueryItem("x_auth_username", username.toUtf8().toPercentEncoding());
    url.addEncodedQueryItem("x_auth_password", password.toUtf8().toPercentEncoding());
    url.addQueryItem("x_auth_mode", "client_auth");

    QByteArray oauthHeader = generateAuthorizationHeader(url, OAuth::POST);

    QNetworkRequest req(url);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = m_netManager->post(req, QByteArray());
    connect(reply, SIGNAL(finished()), this, SLOT(finishedAuthorization()));
}

/**
 * Called when authorization is finished
 */
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

/**
 *  Starts PIN based OAuth authorization
 */
void OAuthTwitter::authorizePin()
{
    Q_ASSERT(m_netManager != 0);

    QUrl url(TWITTER_REQUEST_TOKEN_URL);

    QByteArray oauthHeader = generateAuthorizationHeader(url, OAuth::POST);

    QNetworkRequest req(url);
    req.setRawHeader(AUTH_HEADER, oauthHeader);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    //enters event loop, simulate blocking io
    QEventLoop q;
    QTimer t;
    t.setSingleShot(true);
    connect(&t, SIGNAL(timeout()), &q, SLOT(quit()));

    QNetworkReply *reply = m_netManager->post(req, QByteArray());
    connect(reply, SIGNAL(finished()), &q, SLOT(quit()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error()));

    t.start(5000);
    q.exec();

    if (t.isActive()) {
        t.stop();
        QByteArray response = reply->readAll();
        parseTokens(response);

        reply->deleteLater();
        requestAuthorization();

    } else {
        qDebug() << "Timeout";
    }
}

/**
 *  Opens authorization url, this will open browser, Twitter will return PIN number
 *
 *  @remarks Override if you want to show another browser.
 *  Please call with returned PIN requestAccessToken slot to get access tokens
 *
 */
void OAuthTwitter::requestAuthorization()
{
    QUrl authorizeUrl(TWITTER_AUTHORIZE_URL);
    authorizeUrl.addEncodedQueryItem("oauth_token", oauthToken());
    authorizeUrl.addEncodedQueryItem("oauth_callback", "oob");

    emit authorizePinAuthenticate();

    QDesktopServices::openUrl(authorizeUrl);
}

/**
 *  Gets access tokens for user entered pin number
 *  @param pin entered pin number
 */
void OAuthTwitter::requestAccessToken(const QString& pin)
{
    Q_ASSERT(m_netManager != 0);

    QUrl url(TWITTER_ACCESS_TOKEN_URL);
    url.addEncodedQueryItem("oauth_verifier", pin.toAscii()); 

    QByteArray oauthHeader = generateAuthorizationHeader(url, OAuth::POST);

    QEventLoop q;
    QTimer t;
    t.setSingleShot(true);

    connect(&t, SIGNAL(timeout()), &q, SLOT(quit()));

    QNetworkRequest req(url);
    req.setRawHeader(AUTH_HEADER, oauthHeader);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QNetworkReply *reply = m_netManager->post(req, QByteArray());
    connect(reply, SIGNAL(finished()), &q, SLOT(quit()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error()));

    t.start(5000);
    q.exec();

    if(t.isActive()){
        QByteArray response = reply->readAll();
        parseTokens(response);
        reply->deleteLater();

        emit authorizePinFinished();
    } else {
        qDebug() << "Timeout";
    }
}
