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

#ifndef LIBREGRID_SERVER_HW_FANCONTROLLER_H
#define LIBREGRID_SERVER_HW_FANCONTROLLER_H

#include <QObject>
#include "Fan.h"

namespace libreGrid
{
namespace server
{
namespace HW
{

/**
 * @todo write docs
 */
class FanController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(uint num_fans READ num_fans CONSTANT)
    Q_PROPERTY(bool has_noise READ has_noise CONSTANT)
    Q_PROPERTY(uint noise READ noise)
    Q_PROPERTY(QString identifier READ identifier CONSTANT)

public:
    /**
     * Destructor
     */
    ~FanController();

    /**
     * Constructor
     *
     * @param parent parent object
     */
    FanController(QObject *parent);

    /**
     * get the number of fans
     */
    virtual uint num_fans() const = 0;

    /**
     * check of we have noise sensor support
     */
    virtual bool has_noise() const = 0;
    
    /**
     * get the identifier
     */
    virtual const QString& identifier() const = 0;
    
    /**
     * get the current noise value
     */
    virtual uint noise() const
    {
        return 0;
    }

    /**
     * check if the instance is valid
     *
     * if there was some issues while opening the device,
     * this will return false and lastError will contain the last error message
     */
    virtual bool isValid() const
    {
        return m_valid;
    }

    /**
     * returns the last error message
     */
    virtual const QString &lastError() const
    {
        return m_errormsg;
    }
    
    /**
     * process the events from the fans
     * 
     * @returns the status of the polling
     * 
     * if something failed, the error message could be found in lastError
     */
    virtual bool process_events() = 0;
    
    /**
     * get the fan with the specified id
     */
    virtual Fan* getFan(uint id) const = 0;

signals:
    /**
     * notifies about new noise values
     */
    void noiseChanged(uint noise);
    
protected:
    QString     m_errormsg;
    bool        m_valid = false;
};

}
}
}

#endif // LIBREGRID_SERVER_HW_FANCONTROLLER_H
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
