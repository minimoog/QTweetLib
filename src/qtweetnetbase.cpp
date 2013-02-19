/* Copyright 2010 Antonie Jovanoski
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

#include <QtDebug>
#include <QThreadPool>
#include <QNetworkReply>
#include "qtweetnetbase.h"
#include "qtweetstatus.h"
#include "qtweetdmstatus.h"
#include "qtweetuser.h"
#include "qtweetlist.h"
#include "qtweetsearchresult.h"
#include "qtweetsearchpageresults.h"
#include "qtweetplace.h"
#include "json/qjsondocument.h"

/**
 *   Constructor
 */
QTweetNetBase::QTweetNetBase(QObject *parent) :
    QObject(parent), m_oauthTwitter(0), m_jsonParsingEnabled(true), m_authentication(true)
{
}

/**
 *   Constructor
 *   @param oauthTwitter OAuth Twitter
 *   @param parent QObject parent
 */
QTweetNetBase::QTweetNetBase(OAuthTwitter *oauthTwitter, QObject *parent) :
        QObject(parent), m_oauthTwitter(oauthTwitter), m_jsonParsingEnabled(true), m_authentication(true)
{

}

/**
 *  Desctructor
 */
QTweetNetBase::~QTweetNetBase()
{
}

/**
 *   Sets OAuth Twitter authorization object
 *   @param oauthTwitter OAuth Twitter object
 */
void QTweetNetBase::setOAuthTwitter(OAuthTwitter *oauthTwitter)
{
    m_oauthTwitter = oauthTwitter;
}

/**
 *   Gets OAuth Twitter authorization object
 *   @return OAuth Twitter object
 */
OAuthTwitter* QTweetNetBase::oauthTwitter() const
{
    return m_oauthTwitter;
}

/**
 *  Gets response
 */
QByteArray QTweetNetBase::response() const
{
    return m_response;
}

/**
 *  Gets last error message
 */
QString QTweetNetBase::lastErrorMessage() const
{
    return m_lastErrorMessage;
}

/**
 *  Enables/disables json parsing
 *  @remarks When disabled only finished and error signals are emited
 */
void QTweetNetBase::setJsonParsingEnabled(bool enable)
{
    m_jsonParsingEnabled = enable;
}

/**
 *  Checks if its json parsing enabled
 */
bool QTweetNetBase::isJsonParsingEnabled() const
{
    return m_jsonParsingEnabled;
}

/**
 *  Enables/disables oauth authentication
 *  @remarks Most of classes requires authentication
 */
void QTweetNetBase::setAuthenticationEnabled(bool enable)
{
    m_authentication = enable;
}

/**
 *  Checks if authentication is enabled
 */
bool QTweetNetBase::isAuthenticationEnabled() const
{
    return m_authentication;
}

/**
 *  Parses json response
 */
void QTweetNetBase::parseJson(const QByteArray &jsonData)
{
    //### TODO error
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

    parseJsonFinished(jsonDoc);
}

/**
 *  Called after response from twitter
 */
void QTweetNetBase::reply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            m_response = reply->readAll();
            emit finished(m_response);

            if (isJsonParsingEnabled())
                parseJson(m_response);
        } else {
            m_response = reply->readAll();

            //dump error
            qDebug() << "Network error: " << reply->error();
            qDebug() << "Error string: " << reply->errorString();
            qDebug() << "Error response: " << m_response;

            //HTTP status code
            int httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

            //### TODO: try to json parse the error response

            switch (httpStatus) {
            case NotModified:
            case BadRequest:
            case Unauthorized:
            case Forbidden:
            case NotFound:
            case NotAcceptable:
            case EnhanceYourCalm:
            case InternalServerError:
            case BadGateway:
            case ServiceUnavailable:
                emit error(static_cast<ErrorCode>(httpStatus), m_lastErrorMessage);
                break;
            default:
                emit error(UnknownError, m_lastErrorMessage);
            }
        }
        reply->deleteLater();
    }
}

/**
 *  Sets last error message
 */
void QTweetNetBase::setLastErrorMessage(const QString &errMsg)
{
    m_lastErrorMessage = errMsg;
}
