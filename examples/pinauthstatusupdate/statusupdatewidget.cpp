/* Copyright (c) 2011, Antonie Jovanoski
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

#include "statusupdatewidget.h"
#include <QtDebug>
#include <QNetworkAccessManager>
#include "myoauthtwitter.h"
#include "qtweetstatusupdate.h"
#include "ui_statusupdatewidget.h"

StatusUpdateWidget::StatusUpdateWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatusUpdateWidget)
{
    ui->setupUi(this);

    m_oauthTwitter = new MyOAuthTwitter(this);
    m_oauthTwitter->setNetworkAccessManager(new QNetworkAccessManager(this));
}

void StatusUpdateWidget::on_authorizeButton_clicked()
{
    ui->updateButton->setEnabled(false);
    m_oauthTwitter->clearTokens();
    ui->infoLabel->setText("Pin Authorization started");
    m_oauthTwitter->authorizePin();

    if (!m_oauthTwitter->oauthToken().isEmpty() && !m_oauthTwitter->oauthTokenSecret().isEmpty()) {
        ui->infoLabel->setText("Pin Auth success!");
        ui->updateButton->setEnabled(true);
    }
}

void StatusUpdateWidget::on_updateButton_clicked()
{
    QTweetStatusUpdate *statusUpdate = new QTweetStatusUpdate(m_oauthTwitter, this);
    statusUpdate->post(ui->updateTextEdit->toPlainText());
    connect(statusUpdate, SIGNAL(postedStatus(QTweetStatus)), this, SLOT(finishedPostedStatus(QTweetStatus)));
    connect(statusUpdate, SIGNAL(error(ErrorCode,QString)), this, SLOT(error()));
}

void StatusUpdateWidget::finishedPostedStatus(const QTweetStatus &status)
{
    QTweetStatusUpdate *statusUpdate = qobject_cast<QTweetStatusUpdate*>(sender());

    if (statusUpdate) {
        ui->infoLabel->setText("Status posted successfull!");
        statusUpdate->deleteLater();
    }
}

void StatusUpdateWidget::error()
{
    QTweetStatusUpdate *statusUpdate = qobject_cast<QTweetStatusUpdate*>(sender());

    if (statusUpdate) {
        ui->infoLabel->setText("Failed posting status");
        statusUpdate->deleteLater();
    }
}

StatusUpdateWidget::~StatusUpdateWidget()
{
    delete ui;
}
