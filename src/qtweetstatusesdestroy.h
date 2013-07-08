//-----------------Generated file---------------


#ifndef QTWEETSTATUSESDESTROY_H
#define QTWEETSTATUSESDESTROY_H

#include "qtweetnetbase.h"

class QTWEETLIBSHARED_EXPORT QTweetStatusesDestroy : public QTweetNetBase
{
    Q_OBJECT
public:
    QTweetStatusesDestroy(QObject *parent = 0);
    QTweetStatusesDestroy(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void fetch(qint64 id, bool trim_user = false);

signals:
    void status(const QTweetStatus& status);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};
#endif
