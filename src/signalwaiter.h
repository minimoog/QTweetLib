/* Copyright (c) 2010, Antonie Jovanoski
 *
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact e-mail: Antonie Jovanoski <minimoog77_at_gmail.com>
 */

/*
 * Based on http://qt.nokia.com/doc/qq/qq27-responsive-guis.html
 */

#ifndef SIGNALWAITER_H
#define SIGNALWAITER_H

#include <QObject>
#include "qtweetlib_global.h"

class QEventLoop;
class QTimer;

class QTWEETLIBSHARED_EXPORT SignalWaiter : public QObject
{
    Q_OBJECT
public:
    SignalWaiter(const QObject* sender, const char* signal);
    bool wait(int msec);

private:
    QTimer *m_timer;
    QEventLoop *m_eventLoop;
};

#endif // SIGNALWAITER_H
