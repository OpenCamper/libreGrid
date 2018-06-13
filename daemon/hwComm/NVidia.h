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

#ifndef LIBREGRID_SERVER_HW_NVIDIA_H
#define LIBREGRID_SERVER_HW_NVIDIA_H

#include <QThread>
#include <QTimer>
#include <spdlog/spdlog.h>
#include "../contrib/nvml.h"
#include "NVidiaSensor.h"

namespace libreGrid
{
namespace server
{
namespace HW
{

/**
 * @todo write docs
 */
class NVidia : public QObject
{
    Q_OBJECT

public:
    /**
     * Constructor
     */
    NVidia();

    /**
     * Destructor
     */
    ~NVidia();

    /**
     * init the NVML library
     *
     * @returns true on success, false otherwise
     */
    bool init();

    /**
     * returns a list of all known sensors
     */
    QList<NVidiaSensor *> sensors() const
    {
        return m_sensors;
    }

    void run();


    void quit()
    {
        m_thread->quit();
    }

    void wait()
    {
        m_thread->wait();
    }

private:
    void poll();

    QList<NVidiaSensor *> m_sensors;
    
    // our thread
    QThread     *m_thread;
    QTimer      *m_timer;

    std::shared_ptr<spdlog::logger> m_log;

};

}
}
}

#endif // LIBREGRID_SERVER_HW_NVIDIA_H
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
