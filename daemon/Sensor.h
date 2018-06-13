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

#ifndef LIBREGRID_SERVER_SENSOR_H
#define LIBREGRID_SERVER_SENSOR_H

#include <QObject>
#include <QMap>
#include <QDBusConnection>

#include "Settings.h"
#include "Temperature.h"
#include "hwComm/Sensor.h"

namespace libreGrid
{
namespace server
{

/**
 * @todo write docs
 */
class Sensor : public QObject
{
    Q_OBJECT

    Q_CLASSINFO("D-Bus Interface", "libre.Grid1.sensor")
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(int type READ type)

public:
    /**
     * Destructor
     */
    ~Sensor();

    /**
     * Constructor
     *
     * @param parent TODO
     */
    Sensor(QDBusConnection &dbus, HW::Sensor *sensor, QObject *parent);

    QString identifier() const;
    
    QString name() const;
    void setName(const QString &name);
    int type() const;

signals:
    void nameChanged(const QString &name);

private:
    // our settings object
    QSettings *m_settings;

    QDBusConnection                 m_dbus;
    HW::Sensor                      *m_sensor;

    QString                         m_dbusPath;
    QMap<QString, Temperature *>    m_temperatures;
};

}
}

#endif // LIBREGRID_SERVER_SENSOR_H
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
