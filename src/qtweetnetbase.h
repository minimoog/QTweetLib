/* Copyright (c) 2010, Antonie Jovanoski
 *
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact e-mail: Antonie Jovanoski <minimoog77_at_gmail.com>
 */

#ifndef QTWEETNETBASE_H
#define QTWEETNETBASE_H

#include <QObject>
#include <QVariantMap>
#include <QByteArray>
#include <QtDebug>
#include "oauthtwitter.h"
#include "qtweetlib_global.h"

class QTweetStatus;
class QTweetUser;
class QTweetDMStatus;
class QTweetList;
class QTweetSearchResult;
class QTweetSearchPageResults;

/*!
    Base class for Twitter API classes
 */
class QTWEETLIBSHARED_EXPORT QTweetNetBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(OAuthTwitter* oauthTwitter READ oauthTwitter WRITE setOAuthTwitter)
    Q_PROPERTY(bool jsonParsing READ isJsonParsingEnabled WRITE setJsonParsingEnabled)
public: 
    QTweetNetBase(QObject *parent = 0);
    QTweetNetBase(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    virtual ~QTweetNetBase();

    enum ErrorCode {
        JsonParsingError = 1,
        UnknownError = 2,
        NotModified = 304,
        BadRequest = 400,
        Unauthorized = 401,
        Forbidden = 403,
        NotFound = 404,
        NotAcceptable = 406,
        EnhanceYourCalm = 420,
        InternalServerError = 500,
        BadGateway = 502,
        ServiceUnavailable = 503
    };

    void setOAuthTwitter(OAuthTwitter* oauthTwitter);
    OAuthTwitter* oauthTwitter() const;

    void setJsonParsingEnabled(bool enable);
    bool isJsonParsingEnabled() const;

    QByteArray response() const;
    QString lastErrorMessage() const;

signals:
    /*!
        Emited when fetching is finished
        \param response Contains the response
     */
    void finished(const QByteArray& response);
    /*! Emited when there is error. You can check error message with lastErrorMessage().
        \param code Error code
        \param errorMsg Error message. If it's empty then error was not standard json twitter api message.
                        In that case check response.
        \remarks DOESN'T emit finished signal
     */
    void error(ErrorCode code, const QString& errorMsg);

protected slots:
    virtual void parsingJsonFinished(const QVariant& json, bool ok, const QString& errorMsg) = 0;
    virtual void reply();

protected:
    QList<QTweetStatus> variantToStatusList(const QVariant& fromParser);
    QList<QTweetDMStatus> variantToDirectMessagesList(const QVariant& fromParser);
    QTweetDMStatus variantMapToDirectMessage(const QVariantMap& var);
    QTweetUser variantMapToUserInfo(const QVariantMap& var);
    QTweetStatus variantMapToStatus(const QVariantMap& var);
    QTweetList variantMapToTweetList(const QVariantMap& var);
    QList<QTweetUser> variantToUserInfoList(const QVariant& fromParser);
    QList<QTweetList> variantToTweetLists(const QVariant& var);
    QTweetSearchResult variantMapToSearchResult(const QVariantMap& var);
    QTweetSearchPageResults variantToSearchPageResults(const QVariant& var);

    void parseJson(const QByteArray& jsonData);
    void setLastErrorMessage(const QString& errMsg);

private:
    OAuthTwitter *m_oauthTwitter;
    QByteArray m_response;
    QString m_lastErrorMessage;
    bool m_jsonParsingEnabled;
};

#endif // QTWEETNETBASE_H
