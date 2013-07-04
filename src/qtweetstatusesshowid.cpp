#include "qtweetstatusesshowid.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include "qtweetstatus.h"
#include "qtweetconvert.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

QTweetStatusesShowId::QTweetStatusesShowId(QObject *parent) : QTweetNetBase(parent)
{
}

QTweetStatusesShowId::QTweetStatusesShowId(OAuthTwitter *oauthTwitter, QObject *parent) : QTweetNetBase(oauthTwitter, parent)
{
}

void QTweetStatusesShowId::fetch(qint64 id, bool trim_user, bool include_my_retweet, bool include_entities)
{
    if (!isAuthenticationEnabled()) {
        qCritical("Needs authentication to be enabled");
        return;
    }

    QUrl url("https://api.twitter.com/1.1/statuses/show.json");
    QUrlQuery urlQuery;

    urlQuery.addQueryItem("id", QString::number(id));

    if (trim_user)
        urlQuery.addQueryItem("trim_user", "true");

    if (include_my_retweet)
        urlQuery.addQueryItem("include_my_retweet", "true");

    if (include_entities)
        urlQuery.addQueryItem("include_entities", "true");

    url.setQuery(urlQuery);

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetStatusesShowId::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isObject()) {
        QTweetStatus parsedStatus = QTweetConvert::jsonObjectToStatus(jsonDoc.object());

        emit status(parsedStatus);
    }
}
