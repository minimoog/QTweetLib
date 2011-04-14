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

#include "pindialog.h"
#include "ui_pindialog.h"

PinDialog::PinDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PinDialog)
{
    ui->setupUi(this);
}

int PinDialog::pin() const
{
    return ui->pinLineEdit->text().toInt();
}

PinDialog::~PinDialog()
{
    delete ui;
}
