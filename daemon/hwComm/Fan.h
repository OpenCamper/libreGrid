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

#ifndef LIBREGRID_SERVER_HW_FAN_H
#define LIBREGRID_SERVER_HW_FAN_H

#include <QObject>

namespace libreGrid
{
namespace server
{
namespace HW
{


class FanController;
/**
 * @todo write docs
 */
class Fan : public QObject
{
    Q_OBJECT
    Q_PROPERTY(uint id READ id CONSTANT)
    Q_PROPERTY(uint speed READ speed)
    Q_PROPERTY(double current READ current)
    Q_PROPERTY(uint rpm READ rpm)

public:
    /**
     * Constructor
     *
     * @param parent TODO
     */
    Fan(FanController *parent);

    /**
     * Destructor
     */
    ~Fan();

    /**
     * returns the last error message
     */
    virtual const QString &lastError() const
    {
        return m_errormsg;
    }

    /**
     * returns the id of the fan
     */
    virtual uint id() const = 0;

    /**
     * returns the current speed of the fan
     */
    virtual uint speed() const = 0;

    /**
     * returns the actual current of the fan
     */
    virtual double current() const = 0;

    /**
     * returns the current rpm of the fan
     */
    virtual uint rpm() const = 0;

public slots:
    /**
     * set the fanspeed
     *
     * @param speed new fan speed in percent
     */
    virtual bool set_speed(uint speed) = 0;

signals:
    /**
     * emitted if the speed of the fan was changed
     */
    void speedChanged(uint speed);
    
    /**
     * emitted if the rpm reading changed
     */
    void rpmChanged(uint rpm);
    
    /**
     * emitted if the current of the fan changed
     */
    void currentChanged(double current);
    
protected:
    void submit_speed(uint speed);
    void submit_readings(uint rpm, double current);

    QString     m_errormsg;

private:
    FanController *m_parent;

};

}
}
}

#endif // LIBREGRID_SERVER_HW_FAN_H
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
