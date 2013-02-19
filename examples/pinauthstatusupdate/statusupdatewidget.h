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

#ifndef STATUSUPDATEWIDGET_H
#define STATUSUPDATEWIDGET_H

#include <QWidget>

namespace Ui {
    class StatusUpdateWidget;
}

class OAuthTwitter;
class QTweetStatus;

class StatusUpdateWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatusUpdateWidget(QWidget *parent = 0);
    ~StatusUpdateWidget();

private slots:
    void on_authorizeButton_clicked();
    void on_updateButton_clicked();
    void on_pinPushButton_clicked();
    void finishedPostedStatus(const QTweetStatus& status);
    void finishedPostedStatusWithMedia(const QTweetStatus& status);
    void authenticateFinished();
    void grantedAccess();
    void error();

    void on_addImagePushButton_clicked();

private:
    Ui::StatusUpdateWidget *ui;
    OAuthTwitter *m_oauthTwitter;
};

#endif // STATUSUPDATEWIDGET_H
