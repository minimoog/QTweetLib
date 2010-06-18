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

#ifndef QTWITTERNETBASE_H
#define QTWITTERNETBASE_H

#include <QObject>
#include <QByteArray>
#include "oauthtwitter.h"

/*!
    Base class for Twitter API classes
 */
class QTWITTERLIBSHARED_EXPORT QtwitterNetBase : public QObject
{
    Q_OBJECT
public:
    enum ResponseType {
        Xml = 0x01,
        JSON = 0x02
    };
    Q_DECLARE_FLAGS(ResponseTypes, ResponseType)

    QtwitterNetBase(QObject *parent = 0);
    QtwitterNetBase(OAuthTwitter *oauthTwitter, QObject *parent = 0);

    void setOAuthTwitter(OAuthTwitter* oauthTwitter);
    OAuthTwitter* oauthTwitter() const;

    virtual QByteArray response() const;

signals:
    void finished(const QByteArray& response);
    void networkError(const QString& errorString);

protected:
    QByteArray m_response;

private:
    OAuthTwitter *m_oauthTwitter;
};

#endif // QTWITTERNETBASE_H
