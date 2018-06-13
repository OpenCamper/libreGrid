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

#include "lmSensorsChip.h"
#include "lmSensorsTemperature.h"

using namespace libreGrid::server::HW;

lmSensorsChip::lmSensorsChip(sensors_chip_name const *chipname, QObject *parent)
    : Sensor(parent), m_chipname(chipname)
{
    // init the logger
    m_log = spdlog::get("lmSensorsChip");

    // buffer to store the readable name of our chip
    char name[100];

    // read the name into our buffer
    SPDLOG_TRACE(m_log, "reading chip name");
    sensors_snprintf_chip_name(name, 100, m_chipname);

    // we scan the chip for the features
    sensors_feature const *feature;

    int f = 0;
    SPDLOG_TRACE(m_log, "checking features");
    while ((feature = sensors_get_features(m_chipname, &f)) != 0) {
        // we only care about TEMP sensors right now
        if (feature->type != SENSORS_FEATURE_TEMP)
            continue;

        SPDLOG_TRACE(m_log, "reading subfeature");
        // now, get the subfeatures
        sensors_subfeature const *subfeature = sensors_get_subfeature(m_chipname, feature, SENSORS_SUBFEATURE_TEMP_INPUT);
        if (subfeature == 0)
            continue;

        // this is a temp in sensor
        SPDLOG_TRACE(m_log, "setting state to valid");
        m_valid = true;

        // create the temperature element
        SPDLOG_TRACE(m_log, "creating temperature instance");
        lmSensorsTemperature *temp = new lmSensorsTemperature(chipname, feature, subfeature, this);

        // store the temperature
        m_temperatures.push_back(temp);
    }

    // if this is a valid sensor, we store our identifier
    if (m_valid)
        m_identifier = name;

}

lmSensorsChip::~lmSensorsChip()
{
    // cleanup
    SPDLOG_TRACE(m_log, "removing all temperature instances");
    for (auto it = m_temperatures.begin(); it != m_temperatures.end(); ++it)
        delete(*it);

    SPDLOG_TRACE(m_log, "instance destroyed");
}

void lmSensorsChip::poll()
{
    // iterate over all temperatures and update them
    SPDLOG_TRACE(m_log,"polling all temperature instances");
    for (auto it = m_temperatures.begin(); it != m_temperatures.end(); ++it)
        (*it)->poll();
}

QList<Temperature *> lmSensorsChip::temperatures() const
{
    // TODO: find a more elegant solution
    QList<Temperature *> tmplist;
    for (auto it = m_temperatures.begin(); it != m_temperatures.end(); ++it)
        tmplist.push_back(*it);

    return tmplist;
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
