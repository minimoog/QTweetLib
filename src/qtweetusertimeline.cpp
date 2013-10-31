#include "qtweetusertimeline.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include "qtweetstatus.h"
#include "qtweetdmstatus.h"
#include "qtweetconvert.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

QTweetUserTimeline::QTweetUserTimeline(QObject *parent) : QTweetNetBase(parent)
{
}

QTweetUserTimeline::QTweetUserTimeline(OAuthTwitter *oauthTwitter, QObject *parent) : QTweetNetBase(oauthTwitter, parent)
{
}

void QTweetUserTimeline::fetch(qint64 user_id, const QString& screen_name, qint64 since_id, int count, qint64 max_id, bool trim_user, bool exclude_replies, bool contributor_details, bool include_rts)
{
    if (!isAuthenticationEnabled()) {
        qCritical("Needs authentication to be enabled");
        return;
    }

    QUrl url("https://api.twitter.com/1.1/statuses/user_timeline.json");
    QUrlQuery urlQuery;

    if (user_id != 0)
        urlQuery.addQueryItem("user_id", QString::number(user_id));

    if (!screen_name.isEmpty())
        urlQuery.addQueryItem("screen_name", screen_name);

    if (since_id != 0)
        urlQuery.addQueryItem("since_id", QString::number(since_id));

    if (count != 0)
        urlQuery.addQueryItem("count", QString::number(count));

    if (max_id != 0)
        urlQuery.addQueryItem("max_id", QString::number(max_id));

    if (trim_user)
        urlQuery.addQueryItem("trim_user", "true");

    if (exclude_replies)
        urlQuery.addQueryItem("exclude_replies", "true");

    if (contributor_details)
        urlQuery.addQueryItem("contributor_details", "true");

    if (include_rts)
        urlQuery.addQueryItem("include_rts", "true");

    url.setQuery(urlQuery);

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetUserTimeline::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isArray()) {
        QList<QTweetStatus> statuses = QTweetConvert::jsonArrayToStatusList(jsonDoc.array());

        emit statusList(statuses);
    }
}
