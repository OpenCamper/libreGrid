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

#ifndef LIBREGRID_SERVER_HW_NVIDIATEMPERATURE_H
#define LIBREGRID_SERVER_HW_NVIDIATEMPERATURE_H

#include "Temperature.h"
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
class NVidiaTemperature : public Temperature
{
    Q_OBJECT

public:
    /**
     * Destructor
     */
    ~NVidiaTemperature();

    /**
     * Constructor
     *
     * @param parent TODO
     */
    NVidiaTemperature(nvmlDevice_t device, nvmlTemperatureSensors_t tempsensor, QObject *parent);

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
     * @todo write docs
     *
     * @return TODO
     */
    double value() const override
    {
        return m_value;
    }
    
    /**
     * check if this temperature sensor is working
     */
    bool isValid() const
    {
        return m_valid;
    }


    /**
     * update the current value
     */
    bool poll();

private:
    nvmlDevice_t                m_device;
    nvmlTemperatureSensors_t    m_tempsensor;
    QString                     m_identifier;
    
    uint                        m_value;
    
    bool                        m_valid = false;
};

}
}
}

#endif // LIBREGRID_SERVER_HW_NVIDIATEMPERATURE_H
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
