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

#ifndef LIBREGRID_SERVER_HW_GRIDV3FAN_H
#define LIBREGRID_SERVER_HW_GRIDV3FAN_H

#include <spdlog/spdlog.h>
#include "Fan.h"
#include "libusbWrapper.h"


namespace libreGrid
{
namespace server
{
namespace HW
{

class GRIDv3;

/**
 * @todo write docs
 */
class GRIDv3Fan : public Fan
{
    Q_OBJECT

public:
    /**
     * Constructor
     *
     * @param parent owning controller
     * @param id id of this fan
     * @param handle usb device handle of the owning controller
     */
    GRIDv3Fan(GRIDv3 *parent, uint id, libusb_device_handle *handle);


    /**
     * Destructor
     */
    ~GRIDv3Fan();

    /**
     * handle the given data for this fan
     *
     * @param data 21 byte buffer with the data from the USB device
     */
    void process_data(uint8_t buffer[21]);

    /**
     * returns the id of the fan
     */
    uint id() const override
    {
        return m_id;
    }

    /**
     * returns the speed of the fan
     */
    uint speed() const override
    {
        return m_speed;
    }

    /**
     * returns the actual current of the fan
     */
    double current() const override
    {
        return (double)m_current / 100;
    }

    /**
     * returns the current rpm of the fan
     */
    uint rpm() const override
    {
        return m_rpm;
    }

public slots:
    /**
     * set the fanspeed
     *
     * @param speed new fan speed in percent
     */
    bool set_speed(uint speed) override;

private:
    uint m_id;
    libusb_device_handle *m_handle;


    uint m_speed   = 0;
    uint m_rpm     = 0;
    // we store the current in the format returned from the device
    uint m_current = 0;
    
    std::shared_ptr<spdlog::logger> m_log;
};

}
}
}

#endif // LIBREGRID_SERVER_HW_GRIDV3FAN_H
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
