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
#include "Temperature.h"
#include "temperatureadaptor.h"

using namespace libreGrid::server;

Temperature::Temperature(QDBusConnection &dbus, const QString &dbus_basePath, HW::Temperature *temp, Sensor *parent)
    : QObject(parent), m_dbus(dbus), m_temperature(temp)
{
    // initalize our settings object
    m_settings = Settings::get(this);
    m_settings->beginGroup("Sensor");
    m_settings->beginGroup(parent->identifier());
    m_settings->beginGroup("Temperature");
    m_settings->beginGroup(m_temperature->identifier());

    // init the adaptor
    (void)new TemperatureAdaptor(this);

    // register us on the dbus
    m_dbusPath = QString("%1/temperatures/%2").arg(dbus_basePath).arg(temp->identifier());
    m_dbus.registerObject(m_dbusPath, this);

}

Temperature::~Temperature()
{
    // remove us from the dbus
    m_dbus.unregisterObject(m_dbusPath);
}

QString Temperature::name() const
{
    return m_settings->value("name", m_temperature->identifier()).toString();
}


void Temperature::setName(const QString &name)
{
    m_settings->setValue("name", name);
    emit nameChanged(name);
}

double Temperature::value() const
{
    return m_temperature->value();
}


// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
