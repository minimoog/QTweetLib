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

#ifndef QTWEETPLACE_H
#define QTWEETPLACE_H

#include <QVariant>
#include <QString>
#include <QList>
#include "qtweetgeoboundingbox.h"
#include "qtweetlib_global.h"


/**
 *   Store place info
 */
class QTWEETLIBSHARED_EXPORT QTweetPlace
{
public:
    enum Type { Poi, Neighborhood, City, Admin, Country };

    QTweetPlace();
    void setName(const QString& name);
    QString name() const;
    void setCountry(const QString& country);
    QString country() const;
    void setCountryCode(const QString& code);
    QString countryCode() const;
    void setID(const QString& id);  // http://api.twitter.com/1/geo/id/<id>.json
    QString id() const;
    void setBoundingBox(const QTweetGeoBoundingBox& box);
    QTweetGeoBoundingBox boundingBox() const;
    void setContainedWithin(const QList<QTweetPlace>& places);
    QList<QTweetPlace> containedWithin() const;
    void setFullName(const QString& name);
    QString fullName() const;
    void setType(Type type);
    Type type() const;

private:
    QString m_name;
    QString m_country;
    QString m_countryCode;
    // ### TODO: Attributes
    QString m_id;
    QTweetGeoBoundingBox m_boundingBox;
    QList<QTweetPlace> m_containedWithin;
    QString m_fullName;
    Type m_type;
};

Q_DECLARE_METATYPE(QTweetPlace)

#endif // QTWEETPLACE_H
