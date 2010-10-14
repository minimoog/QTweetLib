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

#include <QNetworkAccessManager>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qtweetfriendstimeline.h"
#include "qtweetuserstream.h"
#include "qtweetstatus.h"
#include "qtweetuser.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_oauthtwitter(new OAuthTwitter(this)),
    m_userstream(new QTweetUserStream(this))

{
    ui->setupUi(this);

    m_oauthtwitter->setNetworkAccessManager(new QNetworkAccessManager(this));
    m_userstream->setOAuthTwitter(m_oauthtwitter);
}

MainWindow::~MainWindow()
{
    delete ui;
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


void MainWindow::on_authenticateButton_clicked()
{
    m_oauthtwitter->authorizeXAuth(ui->userNameLineEdit->text(), ui->passwordLineEdit->text());

    ui->tokenLabel->setText(m_oauthtwitter->oauthToken());
    ui->tokenSecretLabel->setText(m_oauthtwitter->oauthTokenSecret());
}

void MainWindow::on_fetchFTPushButton_clicked()
{
    qDebug() << "in fetch()";

    QTweetFriendsTimeline *friendsTimeline = new QTweetFriendsTimeline(m_oauthtwitter, this);
    connect(friendsTimeline, SIGNAL(finished(QByteArray)), this, SLOT(finishedFriendsTimeline(QByteArray)));
    //connect(friendsTimeline, SIGNAL(parsedStatuses(QList<QTweetStatus>)), this, SLOT(finishedFT(QList<QTweetStatus>)));

    bool skipUser;

    if (ui->skipUserFTComboBox->currentText() == "false")
        skipUser = false;
    else
        skipUser = true;

    bool includeRts;

    if (ui->includeRtsFTComboBox->currentText() == "false")
        includeRts = false;
    else
        includeRts = true;

    bool includeEntities;

    if (ui->includeEntitiesFTComboBox->currentText() == "false")
        includeEntities = false;
    else
        includeEntities = true;

    friendsTimeline->fetch(ui->sinceidFTLineEdit->text().toLongLong(),
                           ui->maxidFTLineEdit->text().toLongLong(),
                           ui->countFTLineEdit->text().toInt(),
                           ui->pageFTLineEdit->text().toInt(),
                           skipUser,
                           includeRts,
                           includeEntities);
}

//void MainWindow::finishedFT(const QList<QTweetStatus> &statuses)
//{
//    qDebug() << "in finishedFriendsTimelineParsing()";
//      QTweetFriendsTimeline *friendsTimeline = qobject_cast<QTweetFriendsTimeline*>(sender());

//      for (QList<QTweetStatus>::const_iterator i = statuses.begin(); i != statuses.end(); ++i)
//      {
//        qDebug() << (*i).text() << endl;
//      }

//      if (friendsTimeline) {
//        friendsTimeline->deleteLater();
//      }
//}

void MainWindow::finishedFriendsTimeline(const QByteArray& response)
{
    QTweetFriendsTimeline *friendsTimeline = qobject_cast<QTweetFriendsTimeline*>(sender());

    if (friendsTimeline) {
        friendsTimeline->deleteLater();
    }

    ui->responseFTTextBrowser->setPlainText(response);
}

void MainWindow::on_startUserStreamButton_clicked()
{
    m_userstream->startFetching();
    //connect(m_userstream, SIGNAL(parsedStatusesStream(QTweetStatus)), this, SLOT(streamStatuses(QTweetStatus)));
    connect(m_userstream, SIGNAL(stream(QByteArray)), this, SLOT(streamStatuses(QByteArray)));
}

//void MainWindow::streamStatuses(const QTweetStatus &status)
//{
//    ui->responseUserStreamTextEdit->appendPlainText("id: " + QString::number(status.id()));
//    ui->responseUserStreamTextEdit->appendPlainText("text: " + status.text());
//    ui->responseUserStreamTextEdit->appendPlainText("source: " + status.source());
//    ui->responseUserStreamTextEdit->appendPlainText("in reply to status id: " + QString::number(status.inReplyToStatusId()));
//    ui->responseUserStreamTextEdit->appendPlainText("in reply to userid: " + QString::number(status.inReplyToUserId()));
//    ui->responseUserStreamTextEdit->appendPlainText("in reply to screen name: " + status.inReplyToScreenName());
//    ui->responseUserStreamTextEdit->appendPlainText("userid: " + QString::number(status.user().id()));
//    ui->responseUserStreamTextEdit->appendPlainText("name: " + status.user().name());
//    ui->responseUserStreamTextEdit->appendPlainText("screen name: " + status.user().screenName());
//    ui->responseUserStreamTextEdit->appendPlainText("avatar: " + status.user().profileImageUrl());

//    if (status.retweetedStatus().id() != 0) {
//        ui->responseUserStreamTextEdit->appendPlainText("Retweeted Status: ");
//        ui->responseUserStreamTextEdit->appendPlainText("\tid: " + QString::number(status.retweetedStatus().id()));
//        ui->responseUserStreamTextEdit->appendPlainText("\tuserid: " + QString::number(status.retweetedStatus().id()));
//        ui->responseUserStreamTextEdit->appendPlainText("\tname: " + status.retweetedStatus().user().name());
//        ui->responseUserStreamTextEdit->appendPlainText("\tscreen name: " + status.retweetedStatus().user().screenName());
//    }
//    ui->responseUserStreamTextEdit->appendPlainText("------------------------------------------------------");
//}

void MainWindow::streamStatuses(const QByteArray &response)
{
    ui->responseUserStreamTextEdit->appendPlainText(QString(response));
    ui->responseUserStreamTextEdit->appendPlainText("/n##################/n");
}
