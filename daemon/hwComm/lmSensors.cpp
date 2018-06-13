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

#include "lmSensors.h"
#include "lmSensorsChip.h"

#include <QDebug>
#include <sensors/sensors.h>
#include <sensors/error.h>


using namespace libreGrid::server::HW;

lmSensors::lmSensors()
{
    m_log = spdlog::get("lmSensors");
}


lmSensors::~lmSensors()
{
    // cleanup our created instances
    SPDLOG_TRACE(m_log, "removing chip handlers");
    for (auto it = m_chips.begin(); it != m_chips.end(); ++it)
        delete(*it);

    // cleanup lm_sensors
    SPDLOG_TRACE(m_log, "cleanup sensors library");
    sensors_cleanup();
    
    SPDLOG_TRACE(m_log, "instance destroyed");
    m_log->info("lmSensors handler shut down");
}

bool lmSensors::init()
{
    SPDLOG_TRACE(m_log, "initalizing sensors library");
    int res = sensors_init(0);
    if (res != 0) {
        // we could not initialize the sensors library.
        // log it, and destruct the thread
        std::string error_msg;
        switch (res) {
        case -SENSORS_ERR_KERNEL:
            error_msg = "Kernel interface error";
            break;
        case -SENSORS_ERR_PARSE:
            error_msg = "General parse error";
            break;
        case -SENSORS_ERR_BUS_NAME:
            error_msg = "Can't parse bus name";
            break;
        default:
            error_msg = "unkown error";
            SPDLOG_DEBUG(m_log, "unknown error code {}", res);
            break;
        }
        m_log->error("failed to initialize the lm_sensors library: {}", error_msg);
        // return false, and tell the caller we fucked up
        return false;
    }

    // iterate over all chips
    sensors_chip_name const *cn;
    // lm_sensors uses this strange way to iterate over things:
    // increment a int reference ...
    SPDLOG_TRACE(m_log, "scanning all known chips");
    int c = 0;
    while ((cn = sensors_get_detected_chips(0, &c)) != 0) {
        lmSensorsChip *chip = new lmSensorsChip(cn, this);
        // if the chip is not valid, discard it
        if (!chip->isValid()) {
            delete chip;
            continue;
        }

        // store the chip
        m_chips.push_back(chip);
    }

    return true;
}


const QList<lmSensorsChip *> &lmSensors::chips()
{
    return m_chips;
}


void lmSensors::run()
{
    // create a timer
    SPDLOG_TRACE(m_log, "creating timer instance");
    m_timer = new QTimer(this);

    // set the interval to 1 second
    m_timer->setInterval(1000);

    // make sure the timer runs infinity
    m_timer->setSingleShot(false);

    // connect our timer to our poll slot
    SPDLOG_TRACE(m_log, "connecting timeout signal of timer");
    connect(m_timer, &QTimer::timeout, this, &lmSensors::poll);
       
    // start our timer ...
    SPDLOG_TRACE(m_log, "starting timer");
    m_timer->start();

    // move this instance to a new thread
    m_thread = new QThread();
    
    SPDLOG_TRACE(m_log, "connecting thread signals");
    // make sure we clean up the thread after all is done
    connect(m_thread, &QThread::finished, m_thread, &QThread::deleteLater);
    connect(m_thread, &QThread::finished, this,     &QThread::deleteLater);
    
    SPDLOG_TRACE(m_log, "moving to thread");
    moveToThread(m_thread);
    
    // start the thread
    SPDLOG_TRACE(m_log, "starting thread");
    m_thread->start();
    
    m_log->info("lmSensors handler up and running");
}

void lmSensors::poll()
{
    SPDLOG_TRACE(m_log, "polling on all chips");
    for (auto it = m_chips.begin(); it != m_chips.end(); ++it)
        (*it)->poll();
}


// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
