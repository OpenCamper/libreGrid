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

#ifndef LIBREGRID_SERVER_DEVICE_H
#define LIBREGRID_SERVER_DEVICE_H

#include <QObject>
#include <QSettings>
#include <QDBusConnection>

#include "hwComm/FanController.h"

namespace libreGrid
{
namespace server
{

/**
 * @todo write docs
 */
class Device : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "libre.Grid1.device")

    Q_PROPERTY(QString name READ name WRITE set_name)
    Q_PROPERTY(uint numFans READ numFans)
    Q_PROPERTY(bool hasNoiseSensor READ hasNoiseSensor)
    Q_PROPERTY(uint noise READ noise)

public:
    /**
     * Destructor
     */
    ~Device();

    /**
     * Constructor
     *
     * @param parent TODO
     */
    Device(QDBusConnection &dbus, HW::FanController *controller, QObject *parent);
    
    QString identifier() const;

    uint numFans() const;
    bool hasNoiseSensor() const;
    uint noise() const;
    QString name() const;

    void  set_name(QString name);

signals:
    void nameChanged(const QString &name);
    void noiseChanged(uint noise);


private:   
    // our settings object
    QSettings *m_settings;
    
    QDBusConnection m_dbus;
    HW::FanController *m_controller;
    
    QString m_dbusPath;


};

}
}

#endif // LIBREGRID_SERVER_DEVICE_H
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
