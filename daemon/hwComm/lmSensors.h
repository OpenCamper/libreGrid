/*
 * libreGrid - open (linux) software for the NZXT GRID+ V3
 * Copyright (C) 2018  Thomas Berger <loki@lokis-chaos.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBREGRID_SERVER_HW_LMSENSORS_H
#define LIBREGRID_SERVER_HW_LMSENSORS_H

#include <QThread>
#include <QTimer>

#include <spdlog/spdlog.h>

namespace libreGrid
{
namespace server
{
namespace HW
{

class lmSensorsChip;

/**
 * @todo write docs
 */
class lmSensors : public QObject
{
    Q_OBJECT

public:
    /**
     * Constructor
     *
     * @param parent TODO
     */
    lmSensors();

    /**
     * Destructor
     */
    ~lmSensors();

    /**
     * init the lm_sensors library
     */
    bool init();
    
    /**
     * list all known chips
     */
    const QList<lmSensorsChip *> &chips();

    void run();

    
    void quit() {
        m_thread->quit();
    }
    
    void wait() {
        m_thread->wait();
    }

private:
    // update all sensors
    void poll();
    
    // we don't care about chip names, we only need
    // to keep track of the classes we instanciated to
    // clean up at the end
    QList<lmSensorsChip *> m_chips;
       
    // our thread
    QThread     *m_thread;
    QTimer      *m_timer;
    
    // logger
    std::shared_ptr<spdlog::logger> m_log;

};

}
}
}

#endif // LIBREGRID_SERVER_HW_LMSENSORS_H
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
