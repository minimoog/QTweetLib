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

#include "qtweetgeoboundingbox.h"

QTweetGeoBoundingBox::QTweetGeoBoundingBox()
{
}

QTweetGeoBoundingBox::QTweetGeoBoundingBox(const QTweetGeoCoord &topLeft,
                                           const QTweetGeoCoord &topRight,
                                           const QTweetGeoCoord &bottomRight,
                                           const QTweetGeoCoord &bottomLeft)
{
    setTopLeft(topLeft);
    setTopRight(topRight);
    setBottomRight(bottomRight);
    setBottomLeft(bottomLeft);
}

bool QTweetGeoBoundingBox::isValid() const
{
    return m_topLeft.isValid() & m_topRight.isValid() &
            m_bottomRight.isValid() & m_bottomLeft.isValid();
}
