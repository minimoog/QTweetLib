//-----------------Generated file---------------


#ifndef QTWEETSTATUSESRETWEETID_H
#define QTWEETSTATUSESRETWEETID_H

#include "qtweetnetbase.h"

class QTWEETLIBSHARED_EXPORT QTweetStatusesRetweetId : public QTweetNetBase
{
    Q_OBJECT
public:
    QTweetStatusesRetweetId(QObject *parent = 0);
    QTweetStatusesRetweetId(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void fetch(qint64 id, bool trim_user = false);

signals:
    void status(const QTweetStatus& status);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};
#endif
