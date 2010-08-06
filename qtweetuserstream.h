/* Copyright (c) 2010, Antonie Jovanoski
 *
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact e-mail: Antonie Jovanoski <minimoog77_at_gmail.com>
 */

#ifndef QTWEETUSERSTREAM_H
#define QTWEETUSERSTREAM_H

#include <QObject>
#include <QNetworkReply>

class QNetworkAccessManager;
class QAuthenticator;

class QTweetUserStream : public QObject
{
    Q_OBJECT
public:
    QTweetUserStream(QObject *parent = 0);
    void setNetworkAccessManager(QNetworkAccessManager* netManager);
    QNetworkAccessManager* networkAccessManager() const;
    void setUsername(const QString& username);
    void setPassword(const QString& password);
    void startFetching();

signals:
    void stream(const QByteArray& );

public slots:

private slots:
    void authRequired(QNetworkReply* reply, QAuthenticator* authenticator);
    void replyError(QNetworkReply::NetworkError code);
    void replyFinished();
    void replyReadyRead();

private:
    QNetworkAccessManager *m_netManager;
    QNetworkReply *m_reply;
    QString m_username;
    QString m_password;
    QByteArray m_cashedResponse;
};

#endif // QTWEETUSERSTREAM_H
