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

#include <QtDebug>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "qtweetlistgetlists.h"
#include "qtweetlist.h"
#include "qtweetconvert.h"
#include "cJSON.h"

QTweetListGetLists::QTweetListGetLists(QObject *parent) :
    QTweetNetBase(parent)
{
}

QTweetListGetLists::QTweetListGetLists(OAuthTwitter *oauthTwitter, QObject *parent) :
        QTweetNetBase(oauthTwitter, parent)
{
}

/** Gets the lists
 *  @param user user id
 *  @param cursor breaks the results into pages. Provide a value of "-1" to begin paging.
 */
void QTweetListGetLists::getLists(qint64 user,
                                  const QString &cursor)
{
    if (!isAuthenticationEnabled()) {
        qCritical("Needs authentication to be enabled");
        return;
    }

    QUrl url(QString("http://api.twitter.com/1/%1/lists.json").arg(user));

    if (!cursor.isEmpty())
        url.addQueryItem("cursor", cursor);

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetListGetLists::parseJsonFinished(cJSON *root)
{
    cJSON *listsObject = cJSON_GetObjectItem(root, "lists");

    QList<QTweetList> lists = QTweetConvert::cJSONToTweetLists(listsObject);
    QString nextCursor = cJSON_GetObjectItem(root, "next_cursor_str")->valuestring;
    QString prevCursor = cJSON_GetObjectItem(root, "previous_cursor_str")->valuestring;

    emit parsedLists(lists, nextCursor, prevCursor);
}
