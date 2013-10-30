//-----------------Generated file---------------


#ifndef QTWEETDIRECTMESSAGES_H
#define QTWEETDIRECTMESSAGES_H

#include "qtweetnetbase.h"

class QTWEETLIBSHARED_EXPORT QTweetDirectMessages : public QTweetNetBase
{
    Q_OBJECT
public:
    QTweetDirectMessages(QObject *parent = 0);
    QTweetDirectMessages(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void fetch(qint64 since_id = 0, qint64 max_id = 0, int count = 0, bool include_entities = false, bool skip_status = false);

signals:
    void messagesList(const QList<QTweetDMStatus>& messages);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};
#endif
