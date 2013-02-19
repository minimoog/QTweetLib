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

#include "qtweetstatusupdatewithmedia.h"
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QFile>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "json/qjsondocument.h"
#include "json/qjsonobject.h"
#include "qtweetconvert.h"
#include "qtweetstatus.h"

QTweetStatusUpdateWithMedia::QTweetStatusUpdateWithMedia(QObject *parent) :
    QTweetNetBase(parent), m_inReplyToStatusID(0), m_latitude(91.0f),
    m_longitude(181.0f), m_displayCoordinates(false)
{
}

QTweetStatusUpdateWithMedia::QTweetStatusUpdateWithMedia(OAuthTwitter *oauthTwitter, QObject *parent) :
    QTweetNetBase(oauthTwitter, parent), m_inReplyToStatusID(0), m_latitude(91.0f),
    m_longitude(181.0f), m_displayCoordinates(false)
{
}

void QTweetStatusUpdateWithMedia::post()
{
    if (!isAuthenticationEnabled()) {
        qCritical("Needs authentication to be enabled");
        return;
    }

    QUrl url("https://api.twitter.com/1.1/statuses/update_with_media.json");

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::POST);

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart statusPart;
    statusPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"status\""));
    statusPart.setBody(m_status.toUtf8());
    multiPart->append(statusPart);

    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"media[]\""));
    QFile *file = new QFile(m_imageFilename);
    file->open(QFile::ReadOnly);
    imagePart.setBodyDevice(file);
    file->setParent(multiPart);
    multiPart->append(imagePart);

    if (m_sensitive) {
        QHttpPart possiblySensitivePart;
        possiblySensitivePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"possibly_sensitive\""));
        possiblySensitivePart.setBody("true");
        multiPart->append(possiblySensitivePart);
    }

    if (m_inReplyToStatusID) {
        QHttpPart inReplyToStatusIDPart;
        inReplyToStatusIDPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"in_reply_to_status_id\""));
        inReplyToStatusIDPart.setBody(QString::number(m_inReplyToStatusID).toUtf8());
        multiPart->append(inReplyToStatusIDPart);
    }

    if (m_latitude < 90.0f && m_latitude > -90.0f) {
        QHttpPart latitudePart;
        latitudePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"lat\""));
        latitudePart.setBody(QString::number(m_latitude).toUtf8());
        multiPart->append(latitudePart);
    }

    if (m_longitude < 181.0f && m_longitude > -180.0f) {
        QHttpPart longitudePart;
        longitudePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"long\""));
        longitudePart.setBody(QString::number(m_longitude).toUtf8());
        multiPart->append(longitudePart);
    }

    if (!m_placeID.isEmpty()) {
        QHttpPart placeIDPart;
        placeIDPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"place_id\""));
        placeIDPart.setBody(m_placeID.toUtf8());
        multiPart->append(placeIDPart);
    }

    if (m_displayCoordinates) {
        QHttpPart displayCoordinatesPart;
        displayCoordinatesPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"display_coordinates\""));
        displayCoordinatesPart.setBody("true");
        multiPart->append(displayCoordinatesPart);
    }

    QNetworkRequest req(url);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->post(req, multiPart);
    multiPart->setParent(reply);

    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
}


void QTweetStatusUpdateWithMedia::parseJsonFinished(const QJsonDocument &jsonDoc)
{
    if (jsonDoc.isObject()) {
        QTweetStatus status = QTweetConvert::jsonObjectToStatus(jsonDoc.object());

        emit postedUpdate(status);
    }
}
