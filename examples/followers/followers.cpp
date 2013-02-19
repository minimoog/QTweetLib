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

#include "followers.h"
#include "ui_followers.h"
#include <QDeclarativeContext>
#include <QNetworkAccessManager>
#include "followerslistmodel.h"
#include "oauthtwitter.h"

Followers::Followers(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Followers)
{
    ui->setupUi(this);

    m_oauthTwitter = new OAuthTwitter(this);
    m_oauthTwitter->setNetworkAccessManager(new QNetworkAccessManager(this));
    m_oauthTwitter->setOAuthToken("");
    m_oauthTwitter->setOAuthTokenSecret("");

    m_followersListModel = new FollowersListModel(m_oauthTwitter, this);

    ui->declarativeView->rootContext()->setContextProperty("followersListModel", m_followersListModel);
    ui->declarativeView->setSource(QUrl("qrc:/FollowersList.qml"));

    connect(ui->fetchFollowersPushButton, SIGNAL(clicked()), SLOT(onFetchFollowersPushButtonClicked()));
}

Followers::~Followers()
{
    delete ui;
}

void Followers::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Followers::onFetchFollowersPushButtonClicked()
{
    m_followersListModel->fetchFollowers();
}
