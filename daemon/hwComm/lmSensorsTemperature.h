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

#ifndef LIBREGRID_SERVER_HW_LMSENSORSTEMPERATURE_H
#define LIBREGRID_SERVER_HW_LMSENSORSTEMPERATURE_H

#include "Temperature.h"
#include <sensors/sensors.h>

namespace libreGrid
{
namespace server
{
namespace HW
{

/**
 * @todo write docs
 */
class lmSensorsTemperature : public Temperature
{
    Q_OBJECT

public:
    /**
     * Constructor
     *
     * @param parent TODO
     */
    lmSensorsTemperature(sensors_chip_name const *chipname, sensors_feature const *feature, sensors_subfeature const *subfeature, QObject *parent);

    /**
     * Destructor
     */
    ~lmSensorsTemperature();

    /**
     * get the identifier of the Temperature
     */
    const QString &identifier() const override
    {
        return m_identifier;
    }

    /**
     * update the Temperature value
     */
    void poll();

    /**
     * get the current value
     */
    double value() const override
    {
        return m_value;
    }

private:
    sensors_chip_name const     *m_chipname;
    sensors_feature const       *m_feature;
    sensors_subfeature const    *m_subfeature;

    QString                     m_identifier;

    double                      m_value;

};

}
}
}

#endif // LIBREGRID_SERVER_HW_LMSENSORSTEMPERATURE_H
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
