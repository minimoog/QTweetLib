//-----------------Generated file---------------


#ifndef QTWEETUSERTIMELINE_H
#define QTWEETUSERTIMELINE_H

#include "qtweetnetbase.h"

class QTWEETLIBSHARED_EXPORT QTweetUserTimeline : public QTweetNetBase
{
    Q_OBJECT
public:
    QTweetUserTimeline(QObject *parent = 0);
    QTweetUserTimeline(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    void fetch(qint64 user_id = 0, const QString& screen_name = QString(), qint64 since_id = 0, int count = 0, qint64 max_id = 0, bool trim_user = false, bool exclude_replies = false, bool contributor_details = false, bool include_rts = false);

signals:
    void statusList(const QList<QTweetStatus>& statuses);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);
};
#endif
