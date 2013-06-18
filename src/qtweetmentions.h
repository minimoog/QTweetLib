//-----------------Generated file---------------


#ifndef QTWEETMENTIONS_H
#define QTWEETMENTIONS_H

#include "qtweetnetbase.h"

class QTWEETLIBSHARED_EXPORT QTweetMentions : public QTweetNetBase
{
    Q_OBJECT
public:
    QTweetMentions(QObject *parent = 0);
    QTweetMentions(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void fetch(qint64 since_id = 0, qint64 max_id = 0, int count = 0, bool trim_user = false, bool contributor_details = false, bool include_entities = false);

signals:
    void statusList(const QList<QTweetStatus>& statuses);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};
#endif
