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

#ifndef LIBREGRID_SERVER_HW_NVIDIASENSOR_H
#define LIBREGRID_SERVER_HW_NVIDIASENSOR_H

#include "Sensor.h"
#include "NVidiaTemperature.h"
#include "../contrib/nvml.h"

namespace libreGrid
{
namespace server
{
namespace HW
{

/**
 * @todo write docs
 */
class NVidiaSensor : public Sensor
{
    Q_OBJECT

public:
    /**
     * Constructor
     *
     * @param parent TODO
     */
    NVidiaSensor(nvmlDevice_t device, QObject *parent);

    /**
     * Destructor
     */
    ~NVidiaSensor();

    /**
     * @todo write docs
     *
     * @return TODO
     */
    QList< Temperature * > temperatures() const override;

    /**
     * @todo write docs
     *
     * @return TODO
     */
    const uint type() const override
    {
        return 2;
    }

    /**
     * @todo write docs
     *
     * @return TODO
     */
    const QString &identifier() const override
    {
        return m_identifier;
    }

    /**
     * check if this is a valid, working, sensor
     */
    bool isValid() const
    {
        return m_valid;
    }

    /**
     * update all temperatures
     */
    void poll();

private:
    nvmlDevice_t    m_device;
    QString         m_identifier;
    bool            m_valid = false;

    QList<NVidiaTemperature *> m_temperatures;

};

}
}
}

#endif // LIBREGRID_SERVER_HW_NVIDIASENSOR_H
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
