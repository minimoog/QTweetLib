//-----------------Generated file---------------


#ifndef QTWEETSTATUSESSHOWID_H
#define QTWEETSTATUSESSHOWID_H

#include "qtweetnetbase.h"

class QTWEETLIBSHARED_EXPORT QTweetStatusesShowId : public QTweetNetBase
{
    Q_OBJECT
public:
    QTweetStatusesShowId(QObject *parent = 0);
    QTweetStatusesShowId(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void fetch(qint64 id, bool trim_user = false, bool include_my_retweet = false, bool include_entities = false);

signals:
    void status(const QTweetStatus& status);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};
#endif
