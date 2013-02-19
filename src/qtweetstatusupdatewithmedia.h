/* Copyright 2012 Antonie Jovanoski
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Contact e-mail: Antonie Jovanoski <minimoog77_at_gmail.com>
 */

#ifndef QTWEETSTATUSUPDATEWITHMEDIA_H
#define QTWEETSTATUSUPDATEWITHMEDIA_H

#include "qtweetnetbase.h"
#include "qtweetgeocoord.h"

/**
 * @brief The QTweetStatusUpdateWithMedia class
 *
 * Updates the authenticating user's current status and attaches media for upload.
 */
class QTWEETLIBSHARED_EXPORT QTweetStatusUpdateWithMedia : public QTweetNetBase
{
    Q_OBJECT
    Q_PROPERTY(QString status READ status WRITE setStatus)
    Q_PROPERTY(QString imageFilename READ imageFilename WRITE setImageFilename)
    Q_PROPERTY(bool sensitive READ isSensitive WRITE setSensitive)
    Q_PROPERTY(qint64 inReplyToStatusID READ inReplyToStatusID WRITE setReplyToStatusID)
    Q_PROPERTY(double latitude READ latitude WRITE setLatitude)
    Q_PROPERTY(double longitude READ longitude WRITE setLongitude)
    Q_PROPERTY(QString placeID READ placeID WRITE setPlaceID)
    Q_PROPERTY(bool displayCoordinates READ displayCoordinates WRITE setDisplayCoordinates)

public:
    QTweetStatusUpdateWithMedia(QObject *parent = 0);
    QTweetStatusUpdateWithMedia(OAuthTwitter *oauthTwitter, QObject *parent = 0);

    void post();

    QString status() const { return m_status; }
    void setStatus(const QString& status) { m_status = status; }

    QString imageFilename() const { return m_imageFilename; }
    void setImageFilename(const QString& fn) { m_imageFilename = fn; }

    bool isSensitive() const { return m_sensitive; }
    void setSensitive(bool sensitive) { m_sensitive = sensitive; }

    qint64 inReplyToStatusID() const { return m_inReplyToStatusID; }
    void setReplyToStatusID(qint64 id) { m_inReplyToStatusID = id; }

    double latitude() const { return m_latitude; }
    void setLatitude(double latitude) { m_latitude = latitude; }

    double longitude() const { return m_longitude; }
    void setLongitude(double longitude) { m_longitude = longitude; }

    QString placeID() const { return m_placeID; }
    void setPlaceID(const QString& placeID) { m_placeID = placeID; }

    bool displayCoordinates() const { return m_displayCoordinates; }
    void setDisplayCoordinates(bool enable) { m_displayCoordinates = enable; }

signals:
    /** Emits posted update */
    void postedUpdate(const QTweetStatus& status);

protected slots:
    void parseJsonFinished(const QJsonDocument &jsonDoc);

private:
    QString m_status;
    QString m_imageFilename;
    bool m_sensitive;
    qint64 m_inReplyToStatusID;
    double m_latitude;
    double m_longitude;
    QString m_placeID;
    bool m_displayCoordinates;
};

#endif // QTWEETSTATUSUPDATEWITHMEDIA_H
