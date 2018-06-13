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

#ifndef LIBREGRID_SERVER_CONTROLLER_H
#define LIBREGRID_SERVER_CONTROLLER_H

#include <QObject>
#include <QDBusConnection>
#include <spdlog/spdlog.h>


#include "hwComm/USB.h"
#include "hwComm/lmSensors.h"

#ifdef HAS_NVML
#include "hwComm/NVidia.h"
#endif


#include "Device.h"
#include "Sensor.h"
#include "Profile.h"

namespace libreGrid
{
namespace server
{

/**
 * @todo write docs
 */
class Controller : public QObject
{

    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "libre.Grid1.controller")

    Q_PROPERTY(QStringList availibleDevices READ availibleDevices)
    Q_PROPERTY(QStringList availibleSensors READ availibleSensors)
    Q_PROPERTY(QString     activeProfile    READ activeProfile WRITE setActiveProfile)
    Q_PROPERTY(bool        enabled          READ enabled       WRITE setEnabled)

public:
    /**
     * Constructor
     *
     * @param parent TODO
     */
    Controller(QDBusConnection &dbus, QObject *parent = 0);

    /**
     * Destructor
     */
    ~Controller();

    QStringList availibleDevices() const;
    QStringList availibleSensors() const;
    QString activeProfile() const;
    
    bool enabled() const;
    
    void setEnabled(bool enabled);
    void setActiveProfile(const QString &profile);
    
    /**
     * start up the hardware communication and
     * begin with the real work
     */
    void run();


signals:
    void devicePlugged(const QString &identifier);
    void deviceUnplugged(const QString &identifier);

public slots:
    QString createProfile();
    void deleteProfile(const QString &identifier);
    
private slots:
    // connectors for the usb controller
    void usb_device_plugged(const QString &identifier, HW::FanController* controller);
    void usb_device_unplugged(const QString &identifier);
    
private:
    void initLog();
    
    // our settings object
    QSettings *m_settings;
    
    QDBusConnection         m_dbus;
    HW::USB                 *m_usb_controller;
    HW::lmSensors           *m_lmsensors;
#ifdef HAS_NVML
    HW::NVidia              *m_nvidia;
#endif

    QMap<QString,Device*>   m_devices;
    QMap<QString,Sensor*>   m_sensors;
    
    QMap<QString,Profile*>  m_profiles;
    
    // our logger
    std::shared_ptr<spdlog::logger> m_log;

};

}
}

#endif // LIBREGRID_SERVER_CONTROLLER_H
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
