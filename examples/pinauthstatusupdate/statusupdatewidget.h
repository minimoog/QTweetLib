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
