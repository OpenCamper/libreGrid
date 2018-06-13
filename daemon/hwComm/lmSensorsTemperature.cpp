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

#include <QDebug>
#include "lmSensorsTemperature.h"

using namespace libreGrid::server::HW;

lmSensorsTemperature::lmSensorsTemperature(
    sensors_chip_name const *chipname, sensors_feature const *feature,
    sensors_subfeature const *subfeature, QObject *parent
)
    : Temperature(parent), m_chipname(chipname), m_feature(feature), m_subfeature(subfeature)
{
    // set the identifier from the feature
    m_identifier = sensors_get_label(m_chipname, m_feature);
}


lmSensorsTemperature::~lmSensorsTemperature()
{ }

void lmSensorsTemperature::poll()
{
    double value = 0.0;
    int ret = sensors_get_value(m_chipname, m_subfeature->number, &value);
    if (ret != 0)
        qWarning() << "failed to read data from chip" <<  ret;

    if (value != m_value) {
        m_value = value;
        emit valueChanged(m_value);
    }
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
