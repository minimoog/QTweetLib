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
#include <QNetworkReply>
#include <QNetworkRequest>
#include "qtweetuserlookup.h"
#include "qtweetuser.h"
#include "qtweetconvert.h"
#include "json/qjsondocument.h"
#include "json/qjsonarray.h"

QTweetUserLookup::QTweetUserLookup(QObject *parent) :
    QTweetNetBase(parent)
{
}

QTweetUserLookup::QTweetUserLookup(OAuthTwitter *oauthTwitter, QObject *parent) :
        QTweetNetBase(oauthTwitter, parent)
{
}

/**
 *   Starts fetching
 *   @param useridList list of user IDs
 *   @param screenNameList list of screen names
 */
void QTweetUserLookup::fetch(const QList<qint64> &useridList,
                             const QStringList &screenNameList)
{
    if (!isAuthenticationEnabled()) {
        qCritical("Needs authentication to be enabled");
        return;
    }

    QUrl url("http://api.twitter.com/1/users/lookup.json");

    if (!useridList.isEmpty()) {
        QString idString;

        foreach (qint64 id, useridList)
            idString += QString::number(id) + ',';

        idString.chop(1);

        url.addQueryItem("user_id", idString);
    }

    if (!screenNameList.isEmpty()) {
        QString scrnString;

        foreach (const QString& screenName, screenNameList)
            scrnString += screenName + ',';

        scrnString.chop(1);

        url.addQueryItem("screen_name", scrnString);
    }

    // ### TODO: Add include_entities parameter

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetUserLookup::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isArray()) {
        QList<QTweetUser> userInfoList = QTweetConvert::jsonArrayToUserInfoList(jsonDoc.array());

        emit parsedUserInfoList(userInfoList);
    }
}
