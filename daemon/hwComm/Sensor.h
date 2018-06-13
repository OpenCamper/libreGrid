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

#ifndef LIBREGRID_SERVER_HW_SENSOR_H
#define LIBREGRID_SERVER_HW_SENSOR_H

#include <QObject>
#include "Temperature.h"

namespace libreGrid
{
namespace server
{
namespace HW
{

/**
 * @todo write docs
 */
class Sensor : public QObject
{
    Q_OBJECT

public:
    /**
     * Destructor
     */
    ~Sensor();

    /**
     * Constructor
     *
     * @param parent TODO
     */
    Sensor(QObject *parent);
   
    /**
     * return the identifier of the chip
     */
    virtual const QString &identifier() const = 0;
    
    /**
     * return the type of the sensor
     * 
     * 1 = lm_sensors, 2 = GPU
     */
    virtual const uint type() const = 0;
    
    /**
     * return a list of all temperatures for this sensor
     */
    virtual QList<Temperature*> temperatures() const = 0;

};

}
}
}

#endif // LIBREGRID_SERVER_HW_SENSOR_H
