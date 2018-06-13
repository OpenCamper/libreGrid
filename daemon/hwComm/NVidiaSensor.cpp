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

#include "NVidiaSensor.h"

using namespace libreGrid::server::HW;


NVidiaSensor::NVidiaSensor(nvmlDevice_t device, QObject *parent)
    : Sensor(parent), m_device(device)
{
    // get the name from the device for our identifier
    char dev_uuid[NVML_DEVICE_PART_NUMBER_BUFFER_SIZE];
    int r = nvmlDeviceGetUUID(device, dev_uuid, NVML_DEVICE_PART_NUMBER_BUFFER_SIZE);

    // we can't use devices without unique identifier .. so we just break out
    if (r != NVML_SUCCESS)
        return;

    m_identifier = dev_uuid;

    // check if we have a working sensor on board
    // maybe, later there will be more then just one sensor, we are prepared for this
    NVidiaTemperature *temp = new NVidiaTemperature(m_device, NVML_TEMPERATURE_GPU, this);
    if (temp->isValid())
        m_temperatures.push_back(temp);
    else
        delete temp;


    m_valid = !m_temperatures.empty();
}


NVidiaSensor::~NVidiaSensor()
{
}

QList<Temperature *> NVidiaSensor::temperatures() const
{
    QList<Temperature *> temp;
    for(auto it = m_temperatures.begin(); it != m_temperatures.end(); ++it)
        temp.push_back(*it);
    
    return temp;
}

void NVidiaSensor::poll()
{
    for(auto it = m_temperatures.begin(); it != m_temperatures.end(); ++it)
        (void)((*it)->poll());
}



// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
