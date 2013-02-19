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
#include <QTimer>
#include <QDateTime>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "oauthtwitter.h"
#include "qtweethometimeline.h"
#include "qtweetmentions.h"
#include "qtweetusertimeline.h"
#include "qtweetdirectmessages.h"
#include "qtweetstatus.h"
#include "qtweetdmstatus.h"
#include "qtweetuser.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_sinceidHomeTimeline = 0;
    m_sinceidMentions = 0;
    m_sinceidUserTimeline = 0;
    m_sinceidDirectMessages = 0;

    m_oauthTwitter = new OAuthTwitter(this);
    m_oauthTwitter->setNetworkAccessManager(new QNetworkAccessManager(this));
    connect(m_oauthTwitter, SIGNAL(authorizeXAuthFinished()), this, SLOT(xauthFinished()));
    connect(m_oauthTwitter, SIGNAL(authorizeXAuthError()), this, SLOT(xauthError()));

    m_timer = new QTimer(this);
    m_timer->setInterval(60000);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timerTimeOut()));

    connect(ui->authorizePushButton, SIGNAL(clicked()), this, SLOT(authorizeButtonClicked()));
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

void MainWindow::authorizeButtonClicked()
{
    m_oauthTwitter->authorizeXAuth(ui->usernameLineEdit->text(), ui->passwordLineEdit->text());

}

void MainWindow::xauthFinished()
{
    ui->statusBar->showMessage("xauth succesfull");
    m_timer->start();
    timerTimeOut();
}

void MainWindow::xauthError()
{
    ui->statusBar->showMessage("xauth failed");
}

void MainWindow::timerTimeOut()
{
    QTweetHomeTimeline *homeTimeline = new QTweetHomeTimeline(m_oauthTwitter, this);
    homeTimeline->fetch(m_sinceidHomeTimeline);
    connect(homeTimeline, SIGNAL(parsedStatuses(QList<QTweetStatus>)),
            this, SLOT(homeTimelineStatuses(QList<QTweetStatus>)));

    QTweetMentions *mentions = new QTweetMentions(m_oauthTwitter, this);
    mentions->fetch(m_sinceidMentions);
    connect(mentions, SIGNAL(parsedStatuses(QList<QTweetStatus>)),
            this, SLOT(mentionsStatuses(QList<QTweetStatus>)));

    QTweetUserTimeline *userTimeline = new QTweetUserTimeline(m_oauthTwitter, this);
    userTimeline->fetch(0, QString(), m_sinceidUserTimeline);
    connect(userTimeline, SIGNAL(parsedStatuses(QList<QTweetStatus>)),
            this, SLOT(userTimelineStatuses(QList<QTweetStatus>)));

    QTweetDirectMessages *dmTimeline = new QTweetDirectMessages(m_oauthTwitter, this);
    dmTimeline->fetch(m_sinceidDirectMessages);
    connect(dmTimeline, SIGNAL(parsedDirectMessages(QList<QTweetDMStatus>)),
            this, SLOT(directMessages(QList<QTweetDMStatus>)));
}

void MainWindow::homeTimelineStatuses(const QList<QTweetStatus> &statuses)
{
    QTweetHomeTimeline *homeTimeline = qobject_cast<QTweetHomeTimeline*>(sender());

    if (homeTimeline) {
        if (statuses.count()) {
            //order is messed up, but this is just example
            foreach (const QTweetStatus& status, statuses) {
                ui->homeTimelineTextEdit->append("id: " + QString::number(status.id()));
                ui->homeTimelineTextEdit->append("text: " + status.text());
                ui->homeTimelineTextEdit->append("created: " + status.createdAt().toString());

                QTweetUser userinfo = status.user();

                ui->homeTimelineTextEdit->append("screen name: " + userinfo.screenName());
                ui->homeTimelineTextEdit->append("user id: " + QString::number(userinfo.id()));

                //is it retweet?
                QTweetStatus rtStatus = status.retweetedStatus();

                if (rtStatus.id()) {
                    ui->homeTimelineTextEdit->append("retweet text: " + rtStatus.text());
                }

                ui->homeTimelineTextEdit->append("----------------------------------------");

            }

            m_sinceidHomeTimeline = statuses.at(0).id();
        }

        homeTimeline->deleteLater();
    }
}

void MainWindow::mentionsStatuses(const QList<QTweetStatus> &statuses)
{
    QTweetMentions *mentions = qobject_cast<QTweetMentions*>(sender());

    if (mentions) {
        if (statuses.count()) {
            foreach (const QTweetStatus& status, statuses) {
                ui->mentionsTextEdit->append("id: " + QString::number(status.id()));
                ui->mentionsTextEdit->append("text: " + status.text());
                ui->mentionsTextEdit->append("created: " + status.createdAt().toString());

                QTweetUser userinfo = status.user();

                ui->mentionsTextEdit->append("screen name: " + userinfo.screenName());
                ui->mentionsTextEdit->append("user id: " + QString::number(userinfo.id()));

                ui->mentionsTextEdit->append("----------------------------------------");
            }

            m_sinceidMentions = statuses.at(0).id();
        }
        mentions->deleteLater();
    }
}

void MainWindow::userTimelineStatuses(const QList<QTweetStatus> &statuses)
{
    QTweetUserTimeline *userTimeline = qobject_cast<QTweetUserTimeline*>(sender());

    if (userTimeline) {
        if (statuses.count()) {
            //order is messed up, but this is just example
            foreach (const QTweetStatus& status, statuses) {
                ui->userTimelineTextEdit->append("id: " + QString::number(status.id()));
                ui->userTimelineTextEdit->append("text: " + status.text());
                ui->userTimelineTextEdit->append("created: " + status.createdAt().toString());

                QTweetUser userinfo = status.user();

                ui->userTimelineTextEdit->append("screen name: " + userinfo.screenName());
                ui->userTimelineTextEdit->append("user id: " + QString::number(userinfo.id()));

                ui->userTimelineTextEdit->append("----------------------------------------");
            }

            m_sinceidUserTimeline = statuses.at(0).id();
        }

        userTimeline->deleteLater();
    }
}

void MainWindow::directMessages(const QList<QTweetDMStatus> &directMessages)
{
    QTweetDirectMessages *dmTimeline = qobject_cast<QTweetDirectMessages*>(sender());

    if (dmTimeline) {
        if (directMessages.count()) {
            foreach (const QTweetDMStatus& message, directMessages) {
                ui->directMessagesTextEdit->append("id: " + QString::number(message.id()));
                ui->directMessagesTextEdit->append("text: " + message.text());
                ui->directMessagesTextEdit->append("created: " + message.createdAt().toString());
                ui->directMessagesTextEdit->append("sender: " + message.senderScreenName());
                ui->directMessagesTextEdit->append("sender id: " + QString::number(message.senderId()));

                ui->directMessagesTextEdit->append("----------------------------------------");
            }

            m_sinceidDirectMessages = directMessages.at(0).id();
        }
    }

    dmTimeline->deleteLater();
}
