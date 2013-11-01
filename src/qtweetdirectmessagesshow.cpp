#include "qtweetdirectmessagesshow.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include "qtweetstatus.h"
#include "qtweetdmstatus.h"
#include "qtweetconvert.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

QTweetDirectMessagesShow::QTweetDirectMessagesShow(QObject *parent) : QTweetNetBase(parent)
{
}

QTweetDirectMessagesShow::QTweetDirectMessagesShow(OAuthTwitter *oauthTwitter, QObject *parent) : QTweetNetBase(oauthTwitter, parent)
{
}

void QTweetDirectMessagesShow::fetch(qint64 id)
{
    if (!isAuthenticationEnabled()) {
        qCritical("Needs authentication to be enabled");
        return;
    }

    QUrl url("https://api.twitter.com/1.1/direct_messages/show.json");
    QUrlQuery urlQuery;

    urlQuery.addQueryItem("id", QString::number(id));

    url.setQuery(urlQuery);

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetDirectMessagesShow::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isArray()) {
       QList<QTweetDMStatus> directMessages = QTweetConvert::jsonArrayToDirectMessagesList(jsonDoc.array());

        if (directMessages.size())
            emit message(directMessages.at(0));
    }

    if (jsonDoc.isObject()) {
        QTweetDMStatus dm = QTweetConvert::jsonObjectToDirectMessage(jsonDoc.object());

        emit message(dm);
    }
}
