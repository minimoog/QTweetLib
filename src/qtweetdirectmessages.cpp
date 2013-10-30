#include "qtweetdirectmessages.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include "qtweetstatus.h"
#include "qtweetdmstatus.h"
#include "qtweetconvert.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

QTweetDirectMessages::QTweetDirectMessages(QObject *parent) : QTweetNetBase(parent)
{
}

QTweetDirectMessages::QTweetDirectMessages(OAuthTwitter *oauthTwitter, QObject *parent) : QTweetNetBase(oauthTwitter, parent)
{
}

void QTweetDirectMessages::fetch(qint64 since_id, qint64 max_id, int count, bool include_entities, bool skip_status)
{
    if (!isAuthenticationEnabled()) {
        qCritical("Needs authentication to be enabled");
        return;
    }

    QUrl url("https://api.twitter.com/1.1/direct_messages.json");
    QUrlQuery urlQuery;

    if (since_id != 0)
        urlQuery.addQueryItem("since_id", QString::number(since_id));

    if (max_id != 0)
        urlQuery.addQueryItem("max_id", QString::number(max_id));

    if (count != 0)
        urlQuery.addQueryItem("count", QString::number(count));

    if (include_entities)
        urlQuery.addQueryItem("include_entities", "true");

    if (skip_status)
        urlQuery.addQueryItem("skip_status", "true");

    url.setQuery(urlQuery);

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetDirectMessages::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isArray()) {
        QList<QTweetDMStatus> directMessages = QTweetConvert::jsonArrayToDirectMessagesList(jsonDoc.array());

        emit messagesList(directMessages);
    }
}
