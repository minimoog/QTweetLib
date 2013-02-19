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
#include <QNetworkReply>
#include "qtweetlistsubscribe.h"
#include "qtweetlist.h"
#include "qtweetconvert.h"
#include "json/qjsondocument.h"
#include "json/qjsonobject.h"

QTweetListSubscribe::QTweetListSubscribe(QObject *parent) :
    QTweetNetBase(parent)
{
}

QTweetListSubscribe::QTweetListSubscribe(OAuthTwitter *oauthTwitter, QObject *parent) :
        QTweetNetBase(oauthTwitter, parent)
{
}

/**
 *   @param user user id owner of the list
 *   @param list list id
 */
void QTweetListSubscribe::follow(qint64 user, qint64 list)
{
    if (!isAuthenticationEnabled()) {
        qCritical("Needs authentication to be enabled");
        return;
    }

    QUrl url(QString("http://api.twitter.com/1/%1/%2/subscribers.json").arg(user).arg(list));

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::POST);
    req.setRawHeader(AUTH_HEADER, oauthHeader);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->post(req, QByteArray());
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetListSubscribe::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isObject()) {
        QTweetList list = QTweetConvert::jsonObjectToTweetList(jsonDoc.object());

        emit parsedList(list);
    }
}

