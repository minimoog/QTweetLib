//-----------------Generated file---------------


#ifndef QTWEETDIRECTMESSAGESSENT_H
#define QTWEETDIRECTMESSAGESSENT_H

#include "qtweetnetbase.h"

class QTWEETLIBSHARED_EXPORT QTweetDirectMessagesSent : public QTweetNetBase
{
    Q_OBJECT
public:
    QTweetDirectMessagesSent(QObject *parent = 0);
    QTweetDirectMessagesSent(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void fetch(qint64 since_id = 0, qint64 max_id = 0, int count = 0, int page = 0, bool include_entities = false);

signals:
    void messagesList(const QList<QTweetDMStatus>& messages);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};
#endif
