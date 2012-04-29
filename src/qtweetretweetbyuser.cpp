/* Copyright (c) 2011, Antonie Jovanoski
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

#include "qtweetretweetbyuser.h"
#include <QtDebug>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "qtweetstatus.h"
#include "qtweetconvert.h"
#include "json/qjsondocument.h"
#include "json/qjsonarray.h"

QTweetRetweetByUser::QTweetRetweetByUser(QObject *parent) :
    QTweetNetBase(parent),
    m_userid(0),
    m_sinceid(0),
    m_maxid(0),
    m_count(0),
    m_page(0),
    m_trimUser(false),
    m_includeEntities(false)
{
}

QTweetRetweetByUser::QTweetRetweetByUser(OAuthTwitter *oauthTwitter, QObject *parent) :
    QTweetNetBase(oauthTwitter, parent),
    m_userid(0),
    m_sinceid(0),
    m_maxid(0),
    m_count(0),
    m_page(0),
    m_trimUser(false),
    m_includeEntities(false)
{
}

void QTweetRetweetByUser::fetch(qint64 userid,
                                qint64 sinceid,
                                qint64 maxid,
                                int count,
                                int page,
                                bool trimUser,
                                bool includeEntities)
{
    QUrl url("http://api.twitter.com/1/statuses/retweeted_by_user.json");

    url.addQueryItem("screen_name", QString::number(userid));

    if (sinceid != 0)
        url.addQueryItem("since_id", QString::number(sinceid));

    if (maxid != 0)
        url.addQueryItem("max_id", QString::number(maxid));

    if (count != 0)
        url.addQueryItem("count", QString::number(count));

    if (page != 0)
        url.addQueryItem("page", QString::number(page));

    if (trimUser)
        url.addQueryItem("trim_user", "true");

    if (includeEntities)
        url.addQueryItem("include_entities", "true");

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetRetweetByUser::fetch(const QString &screenName,
                                qint64 sinceid,
                                qint64 maxid,
                                int count,
                                int page,
                                bool trimUser,
                                bool includeEntities)
{
    QUrl url("http://api.twitter.com/1/statuses/retweeted_by_user.json");

    url.addQueryItem("screen_name", screenName);

    if (sinceid != 0)
        url.addQueryItem("since_id", QString::number(sinceid));

    if (maxid != 0)
        url.addQueryItem("max_id", QString::number(maxid));

    if (count != 0)
        url.addQueryItem("count", QString::number(count));

    if (page != 0)
        url.addQueryItem("page", QString::number(page));

    if (trimUser)
        url.addQueryItem("trim_user", "true");

    if (includeEntities)
        url.addQueryItem("include_entities", "true");

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetRetweetByUser::get()
{
    if (m_userid == 0 && m_screenName.isEmpty())
        return;

    if (m_userid)
        fetch(m_userid, m_sinceid, m_maxid, m_count, m_page, m_trimUser, m_includeEntities);
    else
        fetch(m_screenName, m_sinceid, m_maxid, m_count, m_page, m_trimUser, m_includeEntities);
}

void QTweetRetweetByUser::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isArray()) {
        QList<QTweetStatus> statuses = QTweetConvert::jsonArrayToStatusList(jsonDoc.array());

        emit parsedStatuses(statuses);
    }
}
