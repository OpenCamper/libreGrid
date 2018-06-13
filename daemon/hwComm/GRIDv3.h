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

#ifndef LIBREGRID_SERVER_HW_GRIDV3_H
#define LIBREGRID_SERVER_HW_GRIDV3_H

#include <spdlog/spdlog.h>

#include "FanController.h"
#include "USB.h"
#include "GRIDv3Fan.h"

namespace libreGrid
{
namespace server
{
namespace HW
{

/**
 * Implementation for the GRIDv3 Controller.
 *
 * All device specific communication is done within this class.
 *
 */
class GRIDv3 : public FanController
{
    Q_OBJECT

public:
    /**
     * Constructor
     *
     * @param parent parent object
     * @param handle the libusb handle for the device
     */
    GRIDv3(USB *parent, libusb_device *device);

    /**
     * Destructor
     */
    ~GRIDv3();

    /**
     * get the number of fans
     */
    uint num_fans() const override
    {
        return 6;
    }

    /**
     * check of we have noise sensor support
     */
    bool has_noise() const override
    {
        return true;
    }

    /**
     * get the device identifier (serial)
     */
    const QString &identifier() const override
    {
        return m_serial;
    }

    /**
     * returns the current noise value
     */
    uint noise() const override
    {
        return m_noise;
    }

    /**
     * we process our "events" of the GRIDv3 controller
     */
    bool process_events() override;

    /**
     * get the fan with the specified id
     */
    Fan *getFan(uint id) const override;


private:
    USB                     *m_usb;
    libusb_device           *m_device;
    libusb_device_handle    *m_handle;


    QString                 m_serial;
    uint                    m_noise = 0;
    QList<GRIDv3Fan *>      m_fans;

    // our logger
    std::shared_ptr<spdlog::logger> m_log;
};

}
}
}

#endif // LIBREGRID_SERVER_HW_GRIDV3_H
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
