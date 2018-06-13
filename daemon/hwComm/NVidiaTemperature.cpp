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

#include "NVidiaTemperature.h"

using namespace libreGrid::server::HW;

NVidiaTemperature::NVidiaTemperature(nvmlDevice_t device, nvmlTemperatureSensors_t tempsensor, QObject *parent)
    : Temperature(parent), m_device(device), m_tempsensor(tempsensor)
{
    // give this thing a name
    switch(m_tempsensor) {
        case NVML_TEMPERATURE_GPU:
            m_identifier = "GPU";
            break;
        default:
            m_identifier = "UNKNOWN";
            break;
    }
    // check if this sensor works
    m_valid = poll();
}

NVidiaTemperature::~NVidiaTemperature()
{ }

bool NVidiaTemperature::poll()
{
    uint temp;
    bool success = (nvmlDeviceGetTemperature(m_device, m_tempsensor, &temp) == NVML_SUCCESS);
    // if we are initialized, we may send signals
    if (m_valid && m_value != temp) {
        m_value = temp;
        emit valueChanged(m_value);
    }
    return success;
}


// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
