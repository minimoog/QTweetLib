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

#include "qtweetgeocoord.h"

QTweetGeoCoord::QTweetGeoCoord()
    : m_validLatitude(false), m_validLongitude(false)
{
}

QTweetGeoCoord::QTweetGeoCoord(double latitude, double longitude)
{
    setLatitude(latitude);
    setLongitude(longitude);
}

bool QTweetGeoCoord::isValid() const
{
    return m_validLatitude & m_validLongitude;
}

void QTweetGeoCoord::setLatitude(double latitude)
{
    m_latitude = latitude;

    if (m_latitude > -90.0 || m_latitude < 90.0)
        m_validLatitude = true;
    else
        m_validLatitude = false;
}

void QTweetGeoCoord::setLongitude(double longitude)
{
    m_longitude = longitude;

    if (m_longitude > -180.0 || m_longitude < 180.0)
        m_validLongitude = true;
    else
        m_validLongitude = false;
}
