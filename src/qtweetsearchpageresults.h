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

#ifndef QTWEETSEARCHPAGERESULTS_H
#define QTWEETSEARCHPAGERESULTS_H

#include <QList>
#include <QVariant>
#include <QSharedDataPointer>
#include "qtweetlib_global.h"

class QTweetSearchPageResultsData;
class QTweetSearchResult;

/**
 *  Stores page of a search results
 */
class QTWEETLIBSHARED_EXPORT QTweetSearchPageResults
{
public:
    QTweetSearchPageResults();
    QTweetSearchPageResults(const QTweetSearchPageResults &);
    QTweetSearchPageResults &operator=(const QTweetSearchPageResults &);
    ~QTweetSearchPageResults();

    void setMaxId(qint64 maxid);
    qint64 maxid() const;
    void setNextPage(const QByteArray& nextPage);
    QByteArray nextPage() const;
    void setPage(int numPage);
    int page() const;
    void setQuery(const QByteArray& query);
    QByteArray query() const;
    void setRefreshUrl(const QByteArray& url);
    QByteArray refreshUrl() const;
    void setResults(const QList<QTweetSearchResult>& results);
    QList<QTweetSearchResult> results() const;
    void setResultsPerPage (int count);
    int resultsPerPage() const;
    void setSinceId(qint64 sinceid);
    qint64 sinceid() const;
    void setTotal(int total);
    int total() const;

private:
    QSharedDataPointer<QTweetSearchPageResultsData> data;
};

Q_DECLARE_METATYPE(QTweetSearchPageResults)

#endif // QTWEETSEARCHPAGERESULTS_H
