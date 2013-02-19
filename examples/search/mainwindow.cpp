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

#include <QNetworkAccessManager>
#include <QDateTime>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "oauthtwitter.h"
#include "qtweetsearch.h"
#include "qtweetsearchpageresults.h"
#include "qtweetsearchresult.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_oauthTwitter = new OAuthTwitter(this);
    m_oauthTwitter->setNetworkAccessManager(new QNetworkAccessManager(this));

    connect(ui->startSearchButton, SIGNAL(clicked()), this, SLOT(searchButtonClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::searchButtonClicked()
{
    QTweetSearch *twitSearch = new QTweetSearch(m_oauthTwitter, m_oauthTwitter);
    twitSearch->setAuthenticationEnabled(false);
    twitSearch->start(ui->queryLineEdit->text());
    connect(twitSearch, SIGNAL(parsedPageResults(QTweetSearchPageResults)),
            this, SLOT(finishedSearch(QTweetSearchPageResults)));
}

void MainWindow::finishedSearch(const QTweetSearchPageResults &results)
{
    QTweetSearch *twitSearch = qobject_cast<QTweetSearch*>(sender());

    if (twitSearch) {
        ui->resultsTextEdit->append("maxid: " + QString::number(results.maxid()));
        ui->resultsTextEdit->append("next page: " + results.nextPage());
        ui->resultsTextEdit->append("page: " + QString::number(results.page()));
        ui->resultsTextEdit->append("query: " + results.query());
        ui->resultsTextEdit->append("refresh url: " + results.refreshUrl());
        ui->resultsTextEdit->append("rpp: " + QString::number(results.resultsPerPage()));
        ui->resultsTextEdit->append("sinceid: " + QString::number(results.sinceid()));
        ui->resultsTextEdit->append("total: " + QString::number(results.total()));

        QList<QTweetSearchResult> listResults = results.results();

        ui->resultsTextEdit->append("results: \n");

        foreach (const QTweetSearchResult& singleResult, listResults) {
            ui->resultsTextEdit->append("created: " + singleResult.createdAt().toString());
            ui->resultsTextEdit->append("from user: " + singleResult.fromUser());
            ui->resultsTextEdit->append("id: " + QString::number(singleResult.id()));
            ui->resultsTextEdit->append("lang: " + singleResult.lang());
            ui->resultsTextEdit->append("avatar url: " + singleResult.profileImageUrl());
            ui->resultsTextEdit->append("source: " + singleResult.source());
            ui->resultsTextEdit->append("text: " + singleResult.text());
            ui->resultsTextEdit->append("to user: " + singleResult.toUser());
            ui->resultsTextEdit->append("-------");
        }

        twitSearch->deleteLater();
    }
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
