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
#include <QNetworkRequest>
#include <QAuthenticator>
#include <QTimer>
#include <QThreadPool>
#include "oauthtwitter.h"
#include "qtweetuserstream.h"
#include "qtweetstatus.h"
#include "qtweetdmstatus.h"
#include "qtweetuser.h"
#include "qtweetconvert.h"
#include "cJSON.h"

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
    cJSON *root = cJSON_Parse(data.constData());

    if (root)
        parsingFinished(root);
}

void QTweetUserStream::parsingFinished(cJSON *root)
{
    cJSON *friendsObject = cJSON_GetObjectItem(root, "friends");

    if (friendsObject)
        parseFriendsList(root);

    cJSON *dmObject = cJSON_GetObjectItem(root, "direct_message");

    if (dmObject)
        parseDirectMessage(root);

    cJSON *textObject = cJSON_GetObjectItem(root, "text");

    if (textObject) {
        QTweetStatus status = QTweetConvert::cJSONToStatus(root);
        emit statusesStream(status);
    }

    cJSON *deleteObject = cJSON_GetObjectItem(root, "delete");

    if (deleteObject)
        parseDeleteStatus(root);
}

void QTweetUserStream::parseFriendsList(cJSON *root)
{
    QList<qint64> friends;

    if (root->type == cJSON_Array) {
        int size = cJSON_GetArraySize(root);

        for (int i = 0; i < size; i++) {
            cJSON *friendIDObject = cJSON_GetArrayItem(root, i);
            friends.append((qint64)friendIDObject->valuedouble);
        }

        emit friendsList(friends);
    }
}

void QTweetUserStream::parseDirectMessage(cJSON *root)
{
    QTweetDMStatus dm = QTweetConvert::cJSONToDirectMessage(root);

    emit directMessageStream(dm);
}

void QTweetUserStream::parseDeleteStatus(cJSON *root)
{
    cJSON *deleteStatusObject = cJSON_GetObjectItem(root, "delete");
    cJSON *statusObject = cJSON_GetObjectItem(deleteStatusObject, "status");

    qint64 id = (qint64)cJSON_GetObjectItem(statusObject, "id")->valuedouble;
    qint64 userid = (qint64)cJSON_GetObjectItem(statusObject, "user_id")->valuedouble;

    emit deleteStatusStream(id, userid);
}

void QTweetUserStream::sslErrors(const QList<QSslError> &errors)
{
   QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

   if (reply) {
       reply->ignoreSslErrors();
   }
}
