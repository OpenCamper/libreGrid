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

#ifndef LIBREGRID_SERVER_FAN_H
#define LIBREGRID_SERVER_FAN_H

#include <QObject>
#include <QDBusConnection>

#include "hwComm/Fan.h"
#include "Settings.h"

namespace libreGrid
{
namespace server
{

class Device;

/**
 * @todo write docs
 */
class Fan : public QObject
{
    Q_OBJECT

    Q_CLASSINFO("D-Bus Interface", "libre.Grid1.fan")

    Q_PROPERTY(double current READ current)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(uint rpm READ rpm)
    Q_PROPERTY(uint speed READ speed WRITE setSpeed)

public:
    /**
     * Constructor
     *
     * @param parent TODO
     */
    Fan(QDBusConnection &dbus, HW::Fan *fan, const QString &dbus_base_path, Device *parent);

    /**
     * Destructor
     */
    ~Fan();

    double current() const;
    QString name() const;
    void setName(const QString &name);
    uint rpm() const;
    uint speed() const;
    void setSpeed(uint value);

signals:
    void currentChanged(double current);
    void nameChanged(const QString &name);
    void speedChanged(uint speed);
    void rpmChanged(uint rpm);


private:
    // our settings object
    QSettings       *m_settings;

    QDBusConnection m_dbus;
    HW::Fan         *m_fan;
    QString         m_dbusPath;
};

}
}

#endif // LIBREGRID_SERVER_FAN_H
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
