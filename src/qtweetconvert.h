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
struct cJSON;

/**
 *  Contains static converting functions
 */
class QTweetConvert
{
public:
    static QList<QTweetStatus> cJSONToStatusList(cJSON *root);
    static QTweetStatus cJSONToStatus(cJSON *root);
    static QTweetUser cJSONToUser(cJSON *root);
    static QList<QTweetDMStatus> cJSONToDirectMessagesList(cJSON *root);
    static QTweetDMStatus cJSONToDirectMessage(cJSON *root);
    static QTweetList cJSONToTweetList(cJSON *root);
    static QList<QTweetUser> cJSONToUserInfoList(cJSON *root);
    static QList<QTweetList> cJSONToTweetLists(cJSON *root);
    static QTweetSearchResult cJSONToSearchResult(cJSON *root);
    static QTweetSearchPageResults cJSONToSearchPageResults(cJSON *root);
    static QTweetPlace cJSONToPlace(cJSON *root);
    static QTweetPlace cJSONToPlaceRecursive(cJSON *root);
    static QList<QTweetPlace> cJSONToPlaceList(cJSON *root);
    static QTweetEntityUrl cJSONToEntityUrl(cJSON *root);
    static QTweetEntityHashtag cJSONToEntityHashtag(cJSON *root);
    static QTweetEntityUserMentions cJSONToEntityUserMentions(cJSON *root);
};

#endif // QTWEETCONVERT_H
