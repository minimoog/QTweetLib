#include "qtweetstatusesretweetsid.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include "qtweetstatus.h"
#include "qtweetconvert.h"
#include <QJsonDocument>
#include <QJsonArray>

QTweetStatusesRetweetsId::QTweetStatusesRetweetsId(QObject *parent) : QTweetNetBase(parent)
{
}

QTweetStatusesRetweetsId::QTweetStatusesRetweetsId(OAuthTwitter *oauthTwitter, QObject *parent) : QTweetNetBase(oauthTwitter, parent)
{
}

void QTweetStatusesRetweetsId::fetch(qint64 id, int count, bool trim_user)
{
    if (!isAuthenticationEnabled()) {
        qCritical("Needs authentication to be enabled");
        return;
    }

    QString urlString = QString("https://api.twitter.com/1.1/statuses/retweets/%1.json").arg(id);
    QUrl url(urlString);
    QUrlQuery urlQuery;

    if (count != 0)
        urlQuery.addQueryItem("count", QString::number(count));

    if (trim_user)
        urlQuery.addQueryItem("trim_user", "true");

    url.setQuery(urlQuery);

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetStatusesRetweetsId::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isArray()) {
        QList<QTweetStatus> statuses = QTweetConvert::jsonArrayToStatusList(jsonDoc.array());

        emit statusList(statuses);
    }
}
