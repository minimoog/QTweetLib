//-----------------Generated file---------------


#ifndef QTWEETDIRECTMESSAGESSHOW_H
#define QTWEETDIRECTMESSAGESSHOW_H

#include "qtweetnetbase.h"

class QTWEETLIBSHARED_EXPORT QTweetDirectMessagesShow : public QTweetNetBase
{
    Q_OBJECT
public:
    QTweetDirectMessagesShow(QObject *parent = 0);
    QTweetDirectMessagesShow(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void fetch(qint64 id);

signals:
    void message(const QTweetDMStatus& message);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};
#endif
