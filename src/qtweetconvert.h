/* Copyright (c) 2010, Antonie Jovanoski
 *
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact e-mail: Antonie Jovanoski <minimoog77_at_gmail.com>
 */

#ifndef QTWEETCONVERT_H
#define QTWEETCONVERT_H

#include <QList>

class QTweetStatus;
class QTweetUser;
class QTweetDMStatus;
class QTweetList;
class QTweetSearchResult;
class QTweetSearchPageResults;
class QTweetPlace;
class QTweetEntityUrl;
class QTweetEntityHashtag;
class QTweetEntityUserMentions;

class QJsonArray;
class QJsonObject;

/**
 *  Contains static converting functions
 */
class QTweetConvert
{
public:
    static QList<QTweetStatus> jsonArrayToStatusList(const QJsonArray& jsonArray);
    static QTweetStatus jsonObjectToStatus(const QJsonObject& jsonObject);
    static QTweetUser jsonObjectToUser(const QJsonObject& jsonObject);
    static QList<QTweetDMStatus> jsonArrayToDirectMessagesList(const QJsonArray& jsonArray);
    static QTweetDMStatus jsonObjectToDirectMessage(const QJsonObject& jsonObject);
    static QTweetList jsonObjectToTweetList(const QJsonObject& jsonObject);
    static QList<QTweetUser> jsonArrayToUserInfoList(const QJsonArray& jsonArray);
    static QList<QTweetList> jsonArrayToTweetLists(const QJsonArray& jsonArray);
    static QTweetSearchResult jsonObjectToSearchResult(const QJsonObject& var);
    static QTweetSearchPageResults jsonObjectToSearchPageResults(const QJsonObject& jsonObject);
    static QTweetPlace jsonObjectToPlace(const QJsonObject& var);
    static QTweetPlace jsonObjectToPlaceRecursive(const QJsonObject& jsonObject);
    static QList<QTweetPlace> jsonObjectToPlaceList(const QJsonObject& jsonObject);
    static QTweetEntityUrl jsonObjectToEntityUrl(const QJsonObject& jsonObject);
    static QTweetEntityHashtag jsonObjectToEntityHashtag(const QJsonObject &jsonObject);
    static QTweetEntityUserMentions jsonObjectToEntityUserMentions(const QJsonObject& jsonObject);
};

#endif // QTWEETCONVERT_H
