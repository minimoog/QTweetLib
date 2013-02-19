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

#ifndef QTWEETUSERSTREAM_H
#define QTWEETUSERSTREAM_H

#include <QObject>
#include <QNetworkReply>
#include "qtweetlib_global.h"

#ifdef STREAM_LOGGER
    #include <QFile>
#endif

class QNetworkAccessManager;
class QNetworkReply;
class OAuthTwitter;
class QAuthenticator;
class QTimer;
class QTweetStatus;
class QTweetDMStatus;
class QJsonObject;

/**
 *   Class for fetching user stream
 */
class QTWEETLIBSHARED_EXPORT QTweetUserStream : public QObject
{
    Q_OBJECT
public:
    QTweetUserStream(QObject *parent = 0);
    void setOAuthTwitter(OAuthTwitter* oauthTwitter);
    OAuthTwitter* oauthTwitter() const;

signals:
    /**
     *   Emits stream elements
     */
    void stream(const QByteArray& );
    /**
     *   Emits tweets (parsed) elements from stream
     */
    void statusesStream(const QTweetStatus& status);
    /**
     *   Emits friends list of id of authenticated user.
     *   Emited immediately after connecting to the user stream.
     *   If there is no reconnect it won't be emited again.
     */
    void friendsList(const QList<qint64> friends);
    /**
     *   Emits direct message when is arrived in the stream
     */
    void directMessageStream(const QTweetDMStatus& directMessage);
    /**
     *   Emits deletion of status in the stream
     */
    void deleteStatusStream(qint64 id, qint64 userid);
    /**
     *  Emited when user stream is reconnected after failure
     *  Usefull when user stream connection fails to fetch missed tweets with REST API
     */
    void reconnected();
    /**
     * Emited when user stream doesn't connect and backoff timer reaches maximum value (300 seconds)
     * Usefull when users stream fails to revert to REST API
     */
    void failureConnect();

public slots:
    void startFetching();
    void streamDisconnect();

private slots:
    void replyFinished();
    void replyReadyRead();
    void replyTimeout();
    void sslErrors(const QList<QSslError>& errors);

private:
    void parseStream(const QByteArray& );
    void parseFriendsList(const QJsonObject& streamObject);
    void parseDirectMessage(const QJsonObject &json);
    void parseDeleteStatus(const QJsonObject& json);

    QByteArray m_cachedResponse;
    OAuthTwitter *m_oauthTwitter;
    QNetworkReply *m_reply;
    QTimer *m_backofftimer;
    QTimer *m_timeoutTimer;
    bool m_streamTryingReconnect;

#ifdef STREAM_LOGGER
    QFile m_streamLog;
#endif
};

#endif // QTWEETUSERSTREAM_H
