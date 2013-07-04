//-----------------Generated file---------------


#ifndef QTWEETSTATUSESRETWEETSID_H
#define QTWEETSTATUSESRETWEETSID_H

#include "qtweetnetbase.h"

class QTWEETLIBSHARED_EXPORT QTweetStatusesRetweetsId : public QTweetNetBase
{
    Q_OBJECT
public:
    QTweetStatusesRetweetsId(QObject *parent = 0);
    QTweetStatusesRetweetsId(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void fetch(qint64 id, int count = 0, bool trim_user = false);

signals:
    void statusList(const QList<QTweetStatus>& statuses);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};
#endif
