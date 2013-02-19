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
#include <QThreadPool>
#include "qtweetlistcreate.h"
#include "qtweetlist.h"
#include "qtweetconvert.h"
#include "json/qjsondocument.h"
#include "json/qjsonobject.h"

QTweetListCreate::QTweetListCreate(QObject *parent) :
    QTweetNetBase(parent)
{
}

QTweetListCreate::QTweetListCreate(OAuthTwitter *oauthTwitter, QObject *parent) :
        QTweetNetBase(oauthTwitter, parent)
{
}

/** Creates list
 *  @param user user id
 *  @param name the name of the list
 *  @param mode true for public list, false for private list
 *  @param description the description to give the list.
 */
void QTweetListCreate::create(qint64 user,
                              const QString &name,
                              bool mode,
                              const QString &description)
{
    if (!isAuthenticationEnabled()) {
        qCritical("Needs authentication to be enabled");
        return;
    }

    QUrl url(QString("http://api.twitter.com/1/%1/lists.json").arg(user));

    QUrl urlQuery(url);

    urlQuery.addEncodedQueryItem("name", QUrl::toPercentEncoding(name));

    if (!mode)
        urlQuery.addQueryItem("mode", "private");

    if (!description.isEmpty())
        urlQuery.addEncodedQueryItem("description", QUrl::toPercentEncoding(description));

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(urlQuery, OAuth::POST);
    req.setRawHeader(AUTH_HEADER, oauthHeader);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QByteArray postBody = urlQuery.toEncoded(QUrl::RemoveScheme | QUrl::RemoveAuthority | QUrl::RemovePath);
    postBody.remove(0, 1);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->post(req, postBody);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetListCreate::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isObject()) {
        QTweetList list = QTweetConvert::jsonObjectToTweetList(jsonDoc.object());

        emit parsedList(list);
    }
}
