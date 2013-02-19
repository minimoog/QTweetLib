/* Copyright 2011 Antonie Jovanoski
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

#include "statusupdatewidget.h"
#include <QtDebug>
#include <QNetworkAccessManager>
#include <QFileDialog>
#include "oauthtwitter.h"
#include "qtweetstatus.h"
#include "qtweetstatusupdate.h"
#include "qtweetstatusupdatewithmedia.h"
#include "ui_statusupdatewidget.h"

StatusUpdateWidget::StatusUpdateWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatusUpdateWidget)
{
    ui->setupUi(this);

    m_oauthTwitter = new OAuthTwitter(this);
    m_oauthTwitter->setNetworkAccessManager(new QNetworkAccessManager(this));

    connect(m_oauthTwitter, SIGNAL(authorizePinAuthenticate()), this, SLOT(authenticateFinished()));
    connect(m_oauthTwitter, SIGNAL(authorizePinFinished()), this, SLOT(grantedAccess()));
}

void StatusUpdateWidget::on_authorizeButton_clicked()
{
    ui->updateButton->setEnabled(false);
    m_oauthTwitter->clearTokens();
    ui->infoLabel->setText("Pin Authorization started");
    m_oauthTwitter->authorizePin();
}

void StatusUpdateWidget::authenticateFinished()
{
    ui->infoLabel->setText("Please enter pin!");
}

void StatusUpdateWidget::on_updateButton_clicked()
{
    if (ui->imageFilenameLineEdit->text().isEmpty()) {
        QTweetStatusUpdate *statusUpdate = new QTweetStatusUpdate(m_oauthTwitter, this);
        statusUpdate->post(ui->updateTextEdit->toPlainText());
        connect(statusUpdate, SIGNAL(postedStatus(QTweetStatus)), this, SLOT(finishedPostedStatus(QTweetStatus)));
        connect(statusUpdate, SIGNAL(error(ErrorCode,QString)), this, SLOT(error()));
    } else {
        QTweetStatusUpdateWithMedia *statusMediaUpdate = new QTweetStatusUpdateWithMedia(m_oauthTwitter, this);
        statusMediaUpdate->setStatus(ui->updateTextEdit->toPlainText());
        statusMediaUpdate->setImageFilename(ui->imageFilenameLineEdit->text());
        statusMediaUpdate->post();

        connect(statusMediaUpdate, SIGNAL(postedUpdate(QTweetStatus)),
                this, SLOT(finishedPostedStatusWithMedia(QTweetStatus)));
        connect(statusMediaUpdate, SIGNAL(error(QTweetNetBase::ErrorCode, QString)),
                this, SLOT(error()));
    }
}

void StatusUpdateWidget::on_pinPushButton_clicked()
{
    if (!ui->pinLineEdit->text().isEmpty()) {
        m_oauthTwitter->requestAccessToken(ui->pinLineEdit->text());
    }
}

void StatusUpdateWidget::grantedAccess()
{
    ui->infoLabel->setText("Access granted");
    ui->updateButton->setEnabled(true);
}

void StatusUpdateWidget::finishedPostedStatus(const QTweetStatus &status)
{
    Q_UNUSED(status);

    QTweetStatusUpdate *statusUpdate = qobject_cast<QTweetStatusUpdate*>(sender());

    if (statusUpdate) {
        ui->infoLabel->setText("Status posted successfull!");
        statusUpdate->deleteLater();
    }
}

void StatusUpdateWidget::finishedPostedStatusWithMedia(const QTweetStatus &status)
{
    Q_UNUSED(status);

    QTweetStatusUpdateWithMedia *statusMediaUpdate = qobject_cast<QTweetStatusUpdateWithMedia *>(sender());

    if (statusMediaUpdate) {
        ui->infoLabel->setText("Status with media posted successfull!");
        statusMediaUpdate->deleteLater();
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

void StatusUpdateWidget::on_addImagePushButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open image"));

    if (!filename.isEmpty()) {
        ui->imageFilenameLineEdit->setText(filename);
    }
}
