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

// network byte order is used to transfer the data from device to host
#include <arpa/inet.h>
#include "GRIDv3Fan.h"
#include "usb_macros.h"

#define CHECK_OR_ERRORRET_BOOL(call, msg) \
  CHECK_OR_ACTION(call, m_errormsg = msg; return false )

using namespace libreGrid::server::HW;

GRIDv3Fan::GRIDv3Fan(GRIDv3 *parent, uint id, libusb_device_handle *handle)
    : Fan((FanController *)parent), m_id(id), m_handle(handle)
{
    m_log = spdlog::get("GRIDv3Fan");
}


GRIDv3Fan::~GRIDv3Fan()
{ }


void GRIDv3Fan::process_data(uint8_t buffer[21])
{
    SPDLOG_TRACE(m_log, "processing fan data");
    uint new_rpm, new_current;
    // get a pointer to byte 3+4
    uint16_t *speed_ptr = (uint16_t *)&buffer[3];
    // convert the byte order
    new_rpm = ntohs(*speed_ptr);

    // current
    new_current = buffer[10];

    if (new_rpm != m_rpm) {
        m_rpm = new_rpm;
        SPDLOG_TRACE(m_log, "emitting rpm changed signal");
        emit rpmChanged(m_rpm);
    }

    if (new_current != m_current) {
        m_current = new_current;
        SPDLOG_TRACE(m_log, "emitting current changed signal");
        emit currentChanged((double)m_current / 100);
    }
}

bool GRIDv3Fan::set_speed(uint speed)
{
    SPDLOG_DEBUG(m_log, "setting fanspeed");
    // make sure that the speed is within bounds
    if (speed > 100) {
        m_log->error("fanspeed {} is out of bounds", speed);
        return false;
    }

    // create a buffer to write our data
    uint8_t *buffer = (uint8_t *)malloc(65);
    memset(buffer, 0x00, 65);

    int written;

    // initialize the header
    buffer[0] = 0x02;
    buffer[1] = 0x4d;
    buffer[2] = m_id;
    buffer[4] = speed;

    // send the interrupt to the device
    SPDLOG_TRACE(m_log, "submitting data to usb device");
    CHECK_OR_ERRORRET_BOOL(
        libusb_interrupt_transfer(m_handle, 0x01, buffer, 65, &written, 1000),
        "failed to set speed of fan";
    );

    // update our speed vector for the controller
    if (speed != m_speed) {
        m_speed = speed;
        SPDLOG_TRACE(m_log, "emitting speed changed signal");
        emit speedChanged(m_speed);
    }

    return true;
}



// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
