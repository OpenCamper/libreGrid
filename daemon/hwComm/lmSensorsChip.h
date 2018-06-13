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

#ifndef LIBREGRID_SERVER_HW_LMSENSORSCHIP_H
#define LIBREGRID_SERVER_HW_LMSENSORSCHIP_H

#include <spdlog/spdlog.h>
#include <sensors/sensors.h>
#include "Sensor.h"


namespace libreGrid
{
namespace server
{
namespace HW
{

class lmSensorsTemperature;

/**
 * @todo write docs
 */
class lmSensorsChip : public Sensor
{
    Q_OBJECT

public:
    /**
     * Destructor
     */
    ~lmSensorsChip();

    /**
     * Default constructor
     */
    lmSensorsChip(sensors_chip_name const *chipname, QObject *parent);

    /**
     * returns true if the chip is valid ( working, with temperatures )
     */
    bool isValid() const
    {
        return m_valid;
    }

    /**
     * returns our identifier
     */
    const QString &identifier() const override
    {
        return m_identifier;
    }

    /**
     * update all temperatures
     */
    void poll();


    /**
     * return our type
     */
    const uint type() const override
    {
        return 1;
    }
    
    /**
     * return a list of all temperatures
     */
    QList<Temperature *> temperatures() const override;

private:
    bool                     m_valid = false;
    sensors_chip_name const  *m_chipname;
    QString                  m_identifier;

    // like the lmSensors class, we don't realy care
    // about temperature names, we don't need to access
    // them by name
    QList<lmSensorsTemperature *> m_temperatures;
    
    // our logger
    std::shared_ptr<spdlog::logger> m_log;
};

}
}
}

#endif // LIBREGRID_SERVER_HW_LMSENSORSCHIP_H
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
