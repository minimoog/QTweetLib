/* Copyright 2013 Antonie Jovanoski
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

#include "qtweetdirectmessagesshow.h"
#include <QtDebug>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "qtweetdmstatus.h"
#include "qtweetconvert.h"
#include "json/qjsondocument.h"
#include "json/qjsonarray.h"

/**
 * Constructor
 * @param parent
 */
QTweetDirectMessagesShow::QTweetDirectMessagesShow(QObject *parent) :
    QTweetNetBase(parent)
{
}

/**
 * Constructor
 *
 * @param oauthTwitter OAuthTwitter object
 * @param parent parent QObject
 */
QTweetDirectMessagesShow::QTweetDirectMessagesShow(OAuthTwitter *oauthTwitter, QObject *parent) :
    QTweetNetBase(oauthTwitter, parent)
{
}

/**
 * Starts fetching the direct message
 *
 * @param id id of the direct message to fetch
 */
void QTweetDirectMessagesShow::fetch(qint64 id)
{
    if (!isAuthenticationEnabled()) {
        qCritical("Need authentication to be enabled");
        return;
    }

    QUrl url("https://api.twitter.com/1.1/direct_messages/show.json");
    url.addQueryItem("id", QString::number(id));

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetDirectMessagesShow::parseJsonFinished(const QJsonDocument &jsonDocument)
{
    if (jsonDocument.isArray()) {
        QList<QTweetDMStatus> directMessages = QTweetConvert::jsonArrayToDirectMessagesList(jsonDocument.array());

        if (directMessages.size())
            emit parsedDirectMessage(directMessages.at(0));
    }
}
