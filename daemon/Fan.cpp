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

#include "Fan.h"
#include "Device.h"

#include "fanadaptor.h"


using namespace libreGrid::server;


Fan::Fan(QDBusConnection &dbus, HW::Fan *fan, const QString &dbus_base_path, Device *parent)
    : QObject(parent), m_dbus(dbus), m_fan(fan)
{
    // init our config
    m_settings = Settings::get(this);
    m_settings->beginGroup("Device");
    m_settings->beginGroup(parent->identifier());
    m_settings->beginReadArray("Fans");
    m_settings->setArrayIndex(fan->id());
    
    // init the adaptor
    (void)new FanAdaptor(this);

    // register the fan on the dbus
    m_dbusPath = QString("%1/fans/%2").arg(dbus_base_path).arg(fan->id());
    m_dbus.registerObject(m_dbusPath, this);

    // relay our signals
    connect(fan, &HW::Fan::speedChanged, this, &Fan::speedChanged);
    connect(fan, &HW::Fan::currentChanged, this, &Fan::currentChanged);
    connect(fan, &HW::Fan::rpmChanged, this, &Fan::rpmChanged);
}

Fan::~Fan()
{
    // unregister from dbus
    m_dbus.unregisterObject(m_dbusPath);
}

QString Fan::name() const
{
    return m_settings->value("name",QString("Fan %1").arg(m_fan->id())).toString();
}


void Fan::setName(const QString &name)
{
    m_settings->setValue("name",name);
    emit nameChanged(name);
}


uint Fan::speed() const
{
    return m_fan->speed();
}

void Fan::setSpeed(uint speed)
{
    // we use invokeMethod to make sure we are thread safe
    QMetaObject::invokeMethod(m_fan, "set_speed", Q_ARG(uint, speed));
}


double Fan::current() const
{
    return m_fan->current();
}


uint Fan::rpm() const
{
    return m_fan->rpm();
}


// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
