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

#include "Sensor.h"
#include "sensoradaptor.h"
#include <QDebug>

using namespace libreGrid::server;

Sensor::Sensor(QDBusConnection &dbus, HW::Sensor *sensor, QObject *parent)
    : QObject(parent), m_dbus(dbus), m_sensor(sensor)
{
    // initalize our settings object
    m_settings = Settings::get(this);
    m_settings->beginGroup("Sensor");
    m_settings->beginGroup(m_sensor->identifier());

    // init the adaptor
    (void)new SensorAdaptor(this);

    // register ourself on the dbus
    // we need to replace dashes with underscore, as dashes are not allowed
    m_dbusPath = QString("/libre/Grid1/sensors/%1").arg(m_sensor->identifier()).replace('-', '_');
    m_dbus.registerObject(m_dbusPath, this);

    QList<HW::Temperature *> temps = m_sensor->temperatures();

    // create a child object for every sensor
    for (auto it = temps.begin(); it != temps.end(); ++it)
        m_temperatures[(*it)->identifier()] = new Temperature(m_dbus, m_dbusPath, *it, this);
}


Sensor::~Sensor()
{
    // remove us from the dbus
    m_dbus.unregisterObject(m_dbusPath);
}

QString Sensor::identifier() const
{
    return m_sensor->identifier();
}


QString Sensor::name() const
{
    return m_settings->value("name", m_sensor->identifier()).toString();
}

void Sensor::setName(const QString &name)
{
    m_settings->setValue("name", name);
    emit nameChanged(name);
}

int Sensor::type() const
{
    return m_sensor->type();
}



// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
