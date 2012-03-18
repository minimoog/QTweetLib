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
#include <QNetworkReply>
#include <QNetworkRequest>
#include "qtweetliststatuses.h"
#include "qtweetstatus.h"
#include "qtweetconvert.h"
#include "json/qjsondocument.h"
#include "json/qjsonarray.h"

QTweetListStatuses::QTweetListStatuses(QObject *parent) :
    QTweetNetBase(parent)
{
}

QTweetListStatuses::QTweetListStatuses(OAuthTwitter *oauthTwitter, QObject *parent) :
        QTweetNetBase(oauthTwitter, parent)
{
}

/**
 *   @param user user id
 *   @param list list id
 *   @param sinceid returns results with an ID greater than (that is, more recent than) the specified ID.
 *   @param maxid returns results with an ID less than (that is, older than) or equal to the specified ID.
 *   @param perPage specifies how many tweets per page
 *   @param page    specifies the page of results to retrieve.
 *   @param includeEntities when set to true each tweet will include a node called "entities,"
 *   @remarks Setting parameter to default value will not be put in query
 */
void QTweetListStatuses::fetch(qint64 user,
                               qint64 list,
                               qint64 sinceid,
                               qint64 maxid,
                               int perPage,
                               int page,
                               bool includeEntities)
{
    QUrl url(QString("http://api.twitter.com/1/%1/lists/%2/statuses.json").arg(user).arg(list));

    if (sinceid != 0)
        url.addQueryItem("since_id", QString::number(sinceid));

    if (maxid != 0)
        url.addQueryItem("max_id", QString::number(maxid));

    if (perPage != 0)
        url.addQueryItem("per_page", QString::number(perPage));

    if (page != 0)
        url.addQueryItem("page", QString::number(page));

    if (includeEntities)
        url.addQueryItem("include_entities", "true");

    QNetworkRequest req(url);

    if (isAuthenticationEnabled()) {
        QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
        req.setRawHeader(AUTH_HEADER, oauthHeader);
    }

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetListStatuses::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isArray()) {
        QList<QTweetStatus> statuses = QTweetConvert::jsonArrayToStatusList(jsonDoc.array());

        emit parsedStatuses(statuses);
    }
}
