#include "qtweetdirectmessagesdestroy.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include "qtweetstatus.h"
#include "qtweetdmstatus.h"
#include "qtweetconvert.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

QTweetDirectMessagesDestroy::QTweetDirectMessagesDestroy(QObject *parent) : QTweetNetBase(parent)
{
}

QTweetDirectMessagesDestroy::QTweetDirectMessagesDestroy(OAuthTwitter *oauthTwitter, QObject *parent) : QTweetNetBase(oauthTwitter, parent)
{
}

void QTweetDirectMessagesDestroy::fetch(qint64 id, bool include_entities)
{
    if (!isAuthenticationEnabled()) {
        qCritical("Needs authentication to be enabled");
        return;
    }

    QUrl url("https://api.twitter.com/1.1/direct_messages/destroy.json");
    QUrl urlPost("https://api.twitter.com/1.1/direct_messages/destroy.json");
    QUrlQuery urlQuery;

    urlQuery.addQueryItem("id", QString::number(id));

    if (include_entities)
        urlQuery.addQueryItem("include_entities", "true");

    urlPost.setQuery(urlQuery);

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::POST);
    req.setRawHeader(AUTH_HEADER, oauthHeader);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QByteArray statusPost = urlPost.toEncoded(QUrl::RemoveScheme | QUrl::RemoveAuthority | QUrl::RemovePath);
    statusPost.remove(0, 1);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->post(req, statusPost);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}

void QTweetDirectMessagesDestroy::parseJsonFinished(const QJsonDocument &jsonDoc)
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
