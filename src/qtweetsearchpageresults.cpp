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

#include "qtweetsearchpageresults.h"
#include <QSharedData>
#include "qtweetsearchresult.h"

class QTweetSearchPageResultsData : public QSharedData {
public:
    //float completedIn;        //who cares?
    qint64 maxid;
    QByteArray nextPage;
    int page;
    QByteArray query;
    QByteArray refreshUrl;
    QList<QTweetSearchResult> results;
    int resultsPerPage;
    qint64 sinceid;
    int total;
};

QTweetSearchPageResults::QTweetSearchPageResults() : data(new QTweetSearchPageResultsData)
{
}

QTweetSearchPageResults::QTweetSearchPageResults(const QTweetSearchPageResults &rhs) : data(rhs.data)
{
}

QTweetSearchPageResults &QTweetSearchPageResults::operator=(const QTweetSearchPageResults &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

QTweetSearchPageResults::~QTweetSearchPageResults()
{
}

void QTweetSearchPageResults::setMaxId(qint64 maxid)
{
    data->maxid = maxid;
}

qint64 QTweetSearchPageResults::maxid() const
{
    return data->maxid;
}

void QTweetSearchPageResults::setNextPage(const QByteArray &nextPage)
{
    data->nextPage = nextPage;
}

QByteArray QTweetSearchPageResults::nextPage() const
{
    return data->nextPage;
}

void QTweetSearchPageResults::setPage(int numPage)
{
    data->page = numPage;
}

int QTweetSearchPageResults::page() const
{
    return data->page;
}

void QTweetSearchPageResults::setQuery(const QByteArray &query)
{
    data->query = query;
}

QByteArray QTweetSearchPageResults::query() const
{
    return data->query;
}

void QTweetSearchPageResults::setRefreshUrl(const QByteArray &url)
{
    data->refreshUrl = url;
}

QByteArray QTweetSearchPageResults::refreshUrl() const
{
    return data->refreshUrl;
}

void QTweetSearchPageResults::setResults(const QList<QTweetSearchResult> &results)
{
    data->results = results;
}

QList<QTweetSearchResult> QTweetSearchPageResults::results() const
{
    return data->results;
}

void QTweetSearchPageResults::setResultsPerPage(int count)
{
    data->resultsPerPage = count;
}

int QTweetSearchPageResults::resultsPerPage() const
{
    return data->resultsPerPage;
}

void QTweetSearchPageResults::setSinceId(qint64 sinceid)
{
    data->sinceid = sinceid;
}

qint64 QTweetSearchPageResults::sinceid() const
{
    return data->sinceid;
}

void QTweetSearchPageResults::setTotal(int total)
{
    data->total = total;
}

int QTweetSearchPageResults::total() const
{
    return data->total;
}
