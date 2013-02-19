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
#include <QAuthenticator>
#include <QTimer>
#include <QThreadPool>
#include "json/qjsondocument.h"
#include "json/qjsonobject.h"
#include "json/qjsonarray.h"
#include "oauthtwitter.h"
#include "qtweetuserstream.h"
#include "qtweetstatus.h"
#include "qtweetdmstatus.h"
#include "qtweetuser.h"
#include "qtweetconvert.h"

#define TWITTER_USERSTREAM_URL "https://userstream.twitter.com/2/user.json"

// ### TODO User Agent or X-User-Agent

/**
 *  Constructor
 */
QTweetUserStream::QTweetUserStream(QObject *parent) :
    QObject(parent), m_oauthTwitter(0), m_reply(0),
    m_backofftimer(new QTimer(this)),
    m_timeoutTimer(new QTimer(this)),
    m_streamTryingReconnect(false)
{
    m_backofftimer->setInterval(20000);
    m_backofftimer->setSingleShot(true);
    connect(m_backofftimer, SIGNAL(timeout()), this, SLOT(startFetching()));

    m_timeoutTimer->setInterval(90000);
    connect(m_timeoutTimer, SIGNAL(timeout()), this, SLOT(replyTimeout()));

#ifdef STREAM_LOGGER
    m_streamLog.setFileName("streamlog.txt");
    m_streamLog.open(QIODevice::WriteOnly | QIODevice::Text);
#endif
}

/**
 *  Sets oauth twitter object
 */
void QTweetUserStream::setOAuthTwitter(OAuthTwitter *oauthTwitter)
{
    m_oauthTwitter = oauthTwitter;
}

/**
 *  Gets oauth twitter object
 */
OAuthTwitter* QTweetUserStream::oauthTwitter() const
{
    return m_oauthTwitter;
}

/**
 *   Starts fetching user stream
 */
void QTweetUserStream::startFetching()
{
    if (m_reply != 0) {
        m_reply->abort();
        m_reply->deleteLater();
        m_reply = 0;
    }

    QNetworkRequest req;
    req.setUrl(QUrl(TWITTER_USERSTREAM_URL));

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(req.url(), OAuth::GET);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    m_reply = m_oauthTwitter->networkAccessManager()->get(req);
    connect(m_reply, SIGNAL(finished()), this, SLOT(replyFinished()));
    connect(m_reply, SIGNAL(readyRead()), this, SLOT(replyReadyRead()));
    connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(sslErrors(QList<QSslError>)));

    connect(m_reply, SIGNAL(readyRead()), m_timeoutTimer, SLOT(start()));
    connect(m_reply, SIGNAL(finished()), m_timeoutTimer, SLOT(stop()));
}

/**
 *  Disconnects from stream
 */
void QTweetUserStream::streamDisconnect()
{
    if (m_reply != 0) {
        m_reply->disconnect();
        m_reply->abort();
        m_reply->deleteLater();
        m_reply = 0;
    }
}

/**
 *  Called when connection is finished. Reconnects.
 */
void QTweetUserStream::replyFinished()
{
    qDebug() << "User stream closed ";

    m_streamTryingReconnect = true;

    if (!m_reply->error()) { //no error, reconnect
        qDebug() << "No error, reconnect";

        m_reply->deleteLater();
        m_reply = 0;

        startFetching();
    } else {    //error
        qDebug() << "Error: " << m_reply->error() << ", " << m_reply->errorString();

        m_reply->deleteLater();
        m_reply = 0;

        //if (m_backofftimer->interval() < 20001) {
        //  m_backofftimer->start();
        //  return;
        //}

        //increase back off interval
        int nextInterval = 2 * m_backofftimer->interval();

        if (nextInterval > 300000) {
            nextInterval = 300000;
            emit failureConnect();
        }

        m_backofftimer->setInterval(nextInterval);
        m_backofftimer->start();

        qDebug() << "Exp backoff interval: " << nextInterval;
    }
}

void QTweetUserStream::replyReadyRead()
{
    QByteArray response = m_reply->readAll();

#ifdef STREAM_LOGGER
    m_streamLog.write(response);
    m_streamLog.write("\n");
#endif

    if (m_streamTryingReconnect) {
        emit reconnected();
        m_streamTryingReconnect = false;
    }

    //set backoff timer to initial interval
    m_backofftimer->setInterval(20000);

    QByteArray responseWithPreviousCache = response.prepend(m_cachedResponse);

    int start = 0;
    int end;

    while ((end = responseWithPreviousCache.indexOf('\r', start)) != -1) {
        if (start != end) {
            QByteArray element = responseWithPreviousCache.mid(start, end - start);

            if (!element.isEmpty()) {
                emit stream(element);
                parseStream(element);
            }
        }

        int skip = (response.at(end + 1) == QLatin1Char('\n')) ? 2 : 1;
        start = end + skip;
    }

    //undelimited part just cache it
    m_cachedResponse.clear();

    if (start != responseWithPreviousCache.size()) {
        QByteArray element = responseWithPreviousCache.mid(start);
        if (!element.isEmpty())
            m_cachedResponse = element;
    }
}

void QTweetUserStream::replyTimeout()
{
    qDebug() << "Timeout connection";

    m_reply->abort();
}

void QTweetUserStream::parseStream(const QByteArray& data)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

    if (jsonDoc.isObject()) {
        if (jsonDoc.object().contains("friends"))
            parseFriendsList(jsonDoc.object());
        else if (jsonDoc.object().contains("direct_message"))
            parseDirectMessage(jsonDoc.object());
        else if (jsonDoc.object().contains("text")) {
            QTweetStatus status = QTweetConvert::jsonObjectToStatus(jsonDoc.object());
            emit statusesStream(status);
        } else if (jsonDoc.object().contains("delete")) {
            parseDeleteStatus(jsonDoc.object());
        }
    }
}

void QTweetUserStream::parseFriendsList(const QJsonObject& jsonObject)
{
    QList<qint64> friends;

    QJsonValue jsonValue = jsonObject.value("friends");

    if (jsonValue.isArray()) {
        QJsonArray jsonArray = jsonValue.toArray();

        for (int i = 0; i < jsonArray.size(); ++i) {
            friends.push_back(static_cast<qint64>(jsonArray.at(i).toDouble()));
        }

        emit friendsList(friends);
    }
}

void QTweetUserStream::parseDirectMessage(const QJsonObject& json)
{
    QJsonObject directMessageJson = json["direct_message"].toObject();

    QTweetDMStatus directMessage = QTweetConvert::jsonObjectToDirectMessage(directMessageJson);

    emit directMessageStream(directMessage);
}

void QTweetUserStream::parseDeleteStatus(const QJsonObject &json)
{
    QJsonObject deleteStatusJson = json["delete"].toObject();
    QJsonObject statusJson = deleteStatusJson["status"].toObject();

    qint64 id = static_cast<qint64>(statusJson["id"].toDouble());
    qint64 userid = static_cast<qint64>(statusJson["user_id"].toDouble());

    emit deleteStatusStream(id, userid);
}

void QTweetUserStream::sslErrors(const QList<QSslError> &errors)
{
    Q_UNUSED(errors);

    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (reply) {
        reply->ignoreSslErrors();
    }
}
