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

#include "Settings.h"
#include "Device.h"
#include "deviceadaptor.h"
#include <QMap>

#include "Fan.h"

using namespace libreGrid::server;


Device::Device(QDBusConnection &dbus, HW::FanController *controller, QObject *parent)
    : QObject(parent), m_dbus(dbus), m_controller(controller)
{
    // initalize our settings object
    m_settings = Settings::get(this);
    m_settings->beginGroup("Device");
    m_settings->beginGroup(controller->identifier());
    
    // init the adaptor
    (void)new DeviceAdaptor(this);

    // register this device on the dbus
    m_dbusPath = QString("/libre/Grid1/devices/%1").arg(controller->identifier());
    m_dbus.registerObject(m_dbusPath, this);

    // relay the signals from the hardware interface
    connect(m_controller, &HW::FanController::noiseChanged, this, &Device::noiseChanged);

    // create a Fan interface class for each availible fan
    // prepare the fan configuration
    m_settings->beginWriteArray("Fans",m_controller->num_fans());
    for (uint i = 0; i < m_controller->num_fans(); ++i)
        new Fan(m_dbus, m_controller->getFan(i), m_dbusPath, this);
    m_settings->endArray();
}


Device::~Device()
{
    // unregister ourself from the dbus
    m_dbus.unregisterObject(m_dbusPath);
}

QString Device::identifier() const
{
    return m_controller->identifier();
}


QString Device::name() const
{
    return m_settings->value("name", m_controller->identifier()).toString();
}


void Device::set_name(QString name)
{
    m_settings->setValue("name", name);
    emit nameChanged(name);
}


uint Device::numFans() const
{
    return m_controller->num_fans();
}

bool Device::hasNoiseSensor() const
{
    return m_controller->has_noise();
}

uint Device::noise() const
{
    return m_controller->noise();
}


// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
