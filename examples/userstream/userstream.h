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

#ifndef USERSTREAM_H
#define USERSTREAM_H

#include <QWidget>
#include <QTextStream>
#include <QFile>

namespace Ui {
    class UserStream;
}

class OAuthTwitter;
class QTweetUserStream;
class QTweetStatus;

class UserStream : public QWidget
{
    Q_OBJECT

public:
    explicit UserStream(QWidget *parent = 0);
    ~UserStream();

protected:
    void changeEvent(QEvent *e);

private slots:
    void onConnectButtonClicked();
    void onAuthorizeFinished();
    void onAuthorizeError();
    void stream(const QByteArray& stream);
    void statusStream(const QTweetStatus& tweet);

private:
    Ui::UserStream *ui;
    OAuthTwitter *m_oauthTwitter;
    QTweetUserStream *m_userStream;
    QTextStream m_streamlogger;
    QFile m_file;
};

#endif // USERSTREAM_H
