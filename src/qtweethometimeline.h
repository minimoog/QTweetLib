//-----------------Generated file---------------


#ifndef QTWEETHOMETIMELINE_H
#define QTWEETHOMETIMELINE_H

#include "qtweetnetbase.h"

class QTWEETLIBSHARED_EXPORT QTweetHomeTimeline : public QTweetNetBase
{
    Q_OBJECT
public:
    QTweetHomeTimeline(QObject *parent = 0);
    QTweetHomeTimeline(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void fetch(qint64 since_id = 0, qint64 max_id = 0, int count = 0, bool trim_user = false, bool exclude_replies = false, bool contributor_details = false, bool include_entities = false);

signals:
    void statusList(const QList<QTweetStatus>& statuses);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};
#endif
