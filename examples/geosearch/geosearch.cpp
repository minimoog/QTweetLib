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
#include <QDoubleValidator>
#include "oauthtwitter.h"
#include "qtweetplace.h"
#include "qtweetgeosearch.h"
#include "geosearch.h"
#include "qtweetgeocoord.h"
#include "ui_geosearch.h"

GeoSearch::GeoSearch(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GeoSearch)
{
    ui->setupUi(this);

    m_oauthTwitter = new OAuthTwitter(this);
    m_oauthTwitter->setNetworkAccessManager(new QNetworkAccessManager(this));
    m_oauthTwitter->setOAuthToken("");
    m_oauthTwitter->setOAuthTokenSecret("");

    QDoubleValidator *latValidator = new QDoubleValidator(ui->latitudeLineEdit);
    latValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->latitudeLineEdit->setValidator(latValidator);

    QDoubleValidator *longValidator = new QDoubleValidator(ui->longitudeLineEdit);
    longValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->longitudeLineEdit->setValidator(longValidator);

    connect(ui->searchPushButton, SIGNAL(clicked()), SLOT(onSearchPushButtonClicked()));
}

GeoSearch::~GeoSearch()
{
    delete ui;
}

void GeoSearch::changeEvent(QEvent *e)
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

void GeoSearch::onSearchPushButtonClicked()
{
    QTweetGeoSearch *geoSearch = new QTweetGeoSearch(m_oauthTwitter, this);
    QTweetGeoCoord latLong;
    latLong.setLatitude(ui->latitudeLineEdit->text().toDouble());
    latLong.setLongitude(ui->longitudeLineEdit->text().toDouble());

    geoSearch->search(latLong);
    connect(geoSearch, SIGNAL(parsedPlaces(QList<QTweetPlace>)), SLOT(searchPlacesFinished(QList<QTweetPlace>)));
}

void GeoSearch::searchPlacesFinished(const QList<QTweetPlace> &places)
{
    QTweetGeoSearch *geoSearch = qobject_cast<QTweetGeoSearch*>(sender());

    if (geoSearch) {
        ui->tableWidget->clear();
        ui->tableWidget->setRowCount(places.count());
        ui->tableWidget->setColumnCount(2);

        int row = 0;
        foreach (const QTweetPlace& place, places) {
            QTableWidgetItem *fullname = new QTableWidgetItem(place.fullName());
            ui->tableWidget->setItem(row, 0, fullname);
            QTableWidgetItem *placeid = new QTableWidgetItem(place.id());
            ui->tableWidget->setItem(row, 1, placeid);

            ++row;
        }

        geoSearch->deleteLater();

    }
}
