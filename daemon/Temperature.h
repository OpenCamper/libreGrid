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

#ifndef LIBREGRID_SERVER_TEMPERATURE_H
#define LIBREGRID_SERVER_TEMPERATURE_H

#include <QObject>
#include <QDBusConnection>

#include "Settings.h"
#include "hwComm/Temperature.h"

namespace libreGrid
{
namespace server
{

class Sensor;
    
/**
 * @todo write docs
 */
class Temperature : public QObject
{
    Q_OBJECT

    Q_CLASSINFO("D-Bus Interface", "libre.Grid1.temperature")

    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(double value READ value)

public Q_SLOTS: // METHODS

public:
    /**
     * Destructor
     */
    ~Temperature();

    /**
     * Constructor
     *
     * @param parent TODO
     */
    Temperature(QDBusConnection &dbus, const QString &dbus_basePath, HW::Temperature *temp, Sensor *parent);

    QString name() const;
    void setName(const QString &name);

    double value() const;

signals:
    void nameChanged(const QString &name);
    void valueChanged(double value);

private:
    // our settings object
    QSettings *m_settings;

    QDBusConnection     m_dbus;
    HW::Temperature     *m_temperature;

    QString             m_dbusPath;

};

}
}

#endif // LIBREGRID_SERVER_TEMPERATURE_H
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
