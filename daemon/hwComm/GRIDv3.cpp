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

#include <QVector>
#include <QDebug>
#include <cmath>

#include "GRIDv3.h"
#include "USB.h"
#include "usb_macros.h"


#undef CHECK_OR_ERRORRET
#define CHECK_OR_ERRORRET(call, msg) \
  CHECK_OR_ACTION(call, m_errormsg = msg; return )

#define CHECK_OR_ERRORRET_BOOL(call, msg) \
  CHECK_OR_ACTION(call, m_errormsg = msg; return false )

using namespace libreGrid::server::HW;

GRIDv3::GRIDv3(USB *parent, libusb_device *device)
    : FanController(parent), m_usb(parent), m_device(device)
{
    // get our logger
    m_log = spdlog::get("GRIDv3");

    // open the device
    SPDLOG_TRACE(m_log, "opening usb device");
    CHECK_OR_ERRORRET(libusb_open(device, &m_handle), "failed to open USB device");

    // get the descriptor
    libusb_device_descriptor desc;
    SPDLOG_TRACE(m_log, "getting device descriptor");
    CHECK_OR_ERRORRET(libusb_get_device_descriptor(device, &desc), "failed to get device descriptor");
    // get the serial
    if (!desc.iSerialNumber) {
        m_errormsg = "device has no serial number";
        libusb_close(m_handle);
        return;
    }

    // read the serial string
    SPDLOG_TRACE(m_log, "reading device serial");
    char string[256];
    CHECK_OR_ACTION(
        libusb_get_string_descriptor_ascii(m_handle, desc.iSerialNumber, (uint8_t *)string, sizeof(string)),
        m_errormsg = "failed to retrive device serial";
        libusb_close(m_handle);
        return
    );

    m_serial = string;

    // make sure no kernel module interferres with us
    SPDLOG_TRACE(m_log, "enable auto detach of kernel drivers");
    libusb_set_auto_detach_kernel_driver(m_handle, 1);

    // claim the interface
    SPDLOG_TRACE(m_log, "claiming interface 0");;
    CHECK_OR_ACTION(
        libusb_claim_interface(m_handle, 0),
        m_errormsg = "failed to claim device";
        libusb_close(m_handle);
        return
    )

    // initialize the fans list
    SPDLOG_TRACE(m_log, "creating fan objects");
    m_fans = {
        new GRIDv3Fan(this, 0, m_handle),
        new GRIDv3Fan(this, 1, m_handle),
        new GRIDv3Fan(this, 2, m_handle),
        new GRIDv3Fan(this, 3, m_handle),
        new GRIDv3Fan(this, 4, m_handle),
        new GRIDv3Fan(this, 5, m_handle)
    };

    SPDLOG_DEBUG(m_log, "setting status to valid");
    m_valid = true;
    m_log->info("GRIDv3 controller up and running for device {}", identifier().toStdString());
}



GRIDv3::~GRIDv3()
{
    // clean up our fans
    SPDLOG_TRACE(m_log, "destroying fan objects");
    for (auto it = m_fans.begin(); it != m_fans.end(); ++it)
        delete *it;

    // close our device if we are valid, else it is not open
    if (m_valid) {
        // release the interface
        SPDLOG_TRACE(m_log, "releasing interface 0");
        libusb_release_interface(m_handle, 0);

        // close the device
        SPDLOG_TRACE(m_log, "closing device");
        libusb_close(m_handle);
    }
    SPDLOG_DEBUG(m_log, "instance destroyed");
    m_log->info("GRIDv3 controller shut down for device {}", identifier().toStdString());
}

bool GRIDv3::process_events()
{
    // as the protocol of the GRIDv3+ is passive, we need to poll data for each fan
    // we only know, that we will get one dataset for every fan, but we don't have a specific order


    // our buffer for the fetched data
    uint8_t buffer[21];
    int data_read;

    // our variables to be filled from the buffer
    QVector<uint> noise; // we keep this in a vector to make it easy to calculate the mean value an reduce math-noise ;)

    // we have 6 fans, so we do 6 iterations
    SPDLOG_TRACE(m_log, "reading fan data");
    for (int i = 0; i < 6; ++i) {
        /// @todo we should make sure that broken controllers get a retry and are removed if the error persists
        CHECK_OR_ERRORRET_BOOL(
            libusb_interrupt_transfer(m_handle, 0x81, buffer, 21, &data_read, 1000),
            "failed to read from usb"
        )

        // check if we have fetched all data
        if (data_read != 21) {
            m_log->warn("result of fanstatus request to small for controller {}", identifier().toStdString());
            continue;
        }

        // read the specific blocks of the data to our variables
        uint8_t cur_noise = buffer[1];

        // convert the fan id
        uint8_t fan = ((buffer[15] >> 4) & 0x0f);
        SPDLOG_TRACE(m_log, "read fanID {0:0x} from buffer, converted to {1}", buffer[15], fan);

        // give the buffer to the fan to handle it
        m_fans[fan]->process_data(buffer);
        // push our noise level onto the vector, we will calculate after the loop
        noise.push_back(cur_noise);

        // TODO: publish the data to the fan and send signals
        // emit new_fandata(serial, fan, speed, current);
    }

    // calculate the mean noise level for this controller
    uint new_noise = std::round((double)(std::accumulate(noise.begin(), noise.end(), 0)) / noise.count());
    SPDLOG_TRACE(
        m_log, "calculating noise data from {},{},{},{},{},{}: {}",
        noise[0], noise[1], noise[2], noise[3], noise[4], noise[5], new_noise
    );

    if (new_noise != m_noise) {
        m_noise = new_noise;

        // notify about the change
        SPDLOG_TRACE(m_log, "emitting noise changed signal");
        emit noiseChanged(m_noise);
    }
    return true;
}

Fan *GRIDv3::getFan(uint id) const
{
    return m_fans[id];
}


// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
