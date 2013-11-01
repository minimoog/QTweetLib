//-----------------Generated file---------------


#ifndef QTWEETDIRECTMESSAGESDESTROY_H
#define QTWEETDIRECTMESSAGESDESTROY_H

#include "qtweetnetbase.h"

class QTWEETLIBSHARED_EXPORT QTweetDirectMessagesDestroy : public QTweetNetBase
{
    Q_OBJECT
public:
    QTweetDirectMessagesDestroy(QObject *parent = 0);
    QTweetDirectMessagesDestroy(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void fetch(qint64 id, bool include_entities = false);

signals:
    void message(const QTweetDMStatus& message);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};
#endif
