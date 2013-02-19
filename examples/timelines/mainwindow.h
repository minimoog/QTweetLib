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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class OAuthTwitter;
class QTimer;
class QTweetStatus;
class QTweetDMStatus;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private slots:
    void authorizeButtonClicked();
    void xauthFinished();
    void xauthError();
    void timerTimeOut();
    void homeTimelineStatuses(const QList<QTweetStatus>& statuses);
    void mentionsStatuses(const QList<QTweetStatus>& statuses);
    void userTimelineStatuses(const QList<QTweetStatus>& statuses);
    void directMessages(const QList<QTweetDMStatus>& directMessages);

private:
    Ui::MainWindow *ui;
    OAuthTwitter *m_oauthTwitter;
    QTimer *m_timer;
    qint64 m_sinceidHomeTimeline;
    qint64 m_sinceidMentions;
    qint64 m_sinceidUserTimeline;
    qint64 m_sinceidDirectMessages;

};

#endif // MAINWINDOW_H
