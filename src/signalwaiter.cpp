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

#include <QTimer>
#include <QEventLoop>
#include "signalwaiter.h"

/*!
    Constructor
    \param sender Sender of the signal
    \param signal On which signal to wait
 */
SignalWaiter::SignalWaiter(const QObject* sender, const char* signal) :
    m_timer(new QTimer(this)),
    m_eventLoop(new QEventLoop(this)),
    QObject(0)
{
    m_timer->setSingleShot(true);
    connect(m_timer, SIGNAL(timeout()), m_eventLoop, SLOT(quit()));
    connect(sender, signal, m_eventLoop, SLOT(quit()));
}

/*!
    Wait for the signal, meantime event loop is executed  to not stall UI
    \param msec How many milliseconds to wait for the signal
    \return True if signal emited in waiting time, false otherwise (timeouts)
 */

// ### TODO: Add option for which events to be processed

bool SignalWaiter::wait(int msec)
{
    m_timer->start(msec);
    m_eventLoop->exec();

    if (m_timer->isActive()) {
        m_timer->stop();
        return true;
    } else {
        return false;
    }
}
