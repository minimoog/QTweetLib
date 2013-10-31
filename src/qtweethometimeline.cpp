#include "qtweethometimeline.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include "qtweetstatus.h"
#include "qtweetdmstatus.h"
#include "qtweetconvert.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

QTweetHomeTimeline::QTweetHomeTimeline(QObject *parent) : QTweetNetBase(parent)
{
}

QTweetHomeTimeline::QTweetHomeTimeline(OAuthTwitter *oauthTwitter, QObject *parent) : QTweetNetBase(oauthTwitter, parent)
{
}

void QTweetHomeTimeline::fetch(qint64 since_id, qint64 max_id, int count, bool trim_user, bool exclude_replies, bool contributor_details, bool include_entities)
{
    if (!isAuthenticationEnabled()) {
        qCritical("Needs authentication to be enabled");
        return;
    }

    QUrl url("https://api.twitter.com/1.1/statuses/home_timeline.json");
    QUrlQuery urlQuery;

    if (since_id != 0)
        urlQuery.addQueryItem("since_id", QString::number(since_id));

    if (max_id != 0)
        urlQuery.addQueryItem("max_id", QString::number(max_id));

    if (count != 0)
        urlQuery.addQueryItem("count", QString::number(count));

    if (trim_user)
        urlQuery.addQueryItem("trim_user", "true");

    if (exclude_replies)
        urlQuery.addQueryItem("exclude_replies", "true");

    if (contributor_details)
        urlQuery.addQueryItem("contributor_details", "true");

    if (include_entities)
        urlQuery.addQueryItem("include_entities", "true");

    url.setQuery(urlQuery);

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetHomeTimeline::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isArray()) {
        QList<QTweetStatus> statuses = QTweetConvert::jsonArrayToStatusList(jsonDoc.array());

        emit statusList(statuses);
    }
}
