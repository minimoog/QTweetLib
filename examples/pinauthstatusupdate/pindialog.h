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

#ifndef PINDIALOG_H
#define PINDIALOG_H

#include <QDialog>

namespace Ui {
    class PinDialog;
}

class PinDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PinDialog(QWidget *parent = 0);
    int pin() const;
    ~PinDialog();

private:
    Ui::PinDialog *ui;
};

#endif // PINDIALOG_H
