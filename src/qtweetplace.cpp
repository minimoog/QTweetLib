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

#include "qtweetplace.h"

QTweetPlace::QTweetPlace()
{
}

void QTweetPlace::setName(const QString &name)
{
    m_name = name;
}

QString QTweetPlace::name() const
{
    return m_name;
}

void QTweetPlace::setCountry(const QString &country)
{
    m_country = country;
}

QString QTweetPlace::country() const
{
    return m_country;
}

void QTweetPlace::setCountryCode(const QString &code)
{
    m_countryCode = code;
}

QString QTweetPlace::countryCode() const
{
    return m_countryCode;
}

void QTweetPlace::setID(const QString &id)
{
    m_id = id;
}

QString QTweetPlace::id() const
{
    return m_id;
}

void QTweetPlace::setBoundingBox(const QTweetGeoBoundingBox &box)
{
    m_boundingBox = box;
}

QTweetGeoBoundingBox QTweetPlace::boundingBox() const
{
    return m_boundingBox;
}

void QTweetPlace::setContainedWithin(const QList<QTweetPlace> &places)
{
    m_containedWithin = places;
}

QList<QTweetPlace> QTweetPlace::containedWithin() const
{
    return m_containedWithin;
}

void QTweetPlace::setFullName(const QString &name)
{
    m_fullName = name;
}

QString QTweetPlace::fullName() const
{
    return m_fullName;
}

void QTweetPlace::setType(Type type)
{
    m_type = type;
}

QTweetPlace::Type QTweetPlace::type() const
{
    return m_type;
}
