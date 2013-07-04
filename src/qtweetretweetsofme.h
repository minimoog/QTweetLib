//-----------------Generated file---------------


#ifndef QTWEETRETWEETSOFME_H
#define QTWEETRETWEETSOFME_H

#include "qtweetnetbase.h"

class QTWEETLIBSHARED_EXPORT QTweetRetweetsOfMe : public QTweetNetBase
{
    Q_OBJECT
public:
    QTweetRetweetsOfMe(QObject *parent = 0);
    QTweetRetweetsOfMe(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void fetch(qint64 since_id = 0, qint64 max_id = 0, int count = 0, bool trim_user = false, bool include_entities = false, bool include_user_entities = false);

signals:
    void statusList(const QList<QTweetStatus>& statuses);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};
#endif
