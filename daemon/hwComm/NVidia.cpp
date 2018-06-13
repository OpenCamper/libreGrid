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

#include <QDebug>
#include "NVidia.h"

using namespace libreGrid::server::HW;


NVidia::NVidia()
{
    // get our logger
    m_log = spdlog::get("NVidia");
}


NVidia::~NVidia()
{
    m_log->info("NVidia handler shut down");
}


bool NVidia::init()
{
    // init the nvidia library
    SPDLOG_TRACE(m_log, "initializing NVML");
    nvmlReturn_t r = nvmlInit();

    if (r ==  NVML_ERROR_DRIVER_NOT_LOADED) {
        m_log->info("no NVidia driver loaded, skipping NVidia GPU suport");
        return false;
    }

    if (r != NVML_SUCCESS) {
        m_log->error("failed to init NVML: {}", r);
        return false;
    }

    // get the number of attached nvidia cards
    SPDLOG_TRACE(m_log, "getting number of NVidida devices");
    uint num_devices = 0;
    r = nvmlDeviceGetCount(&num_devices);
    if (r != NVML_SUCCESS) {
        m_log->error("failed to get device count: {}", r);
        return false;
    }
    SPDLOG_DEBUG(m_log, "found {} devices", num_devices);

    // get the handle for each device
    for (uint i = 0; i < num_devices; ++i) {
        SPDLOG_TRACE(m_log, "checking device {}", i);
        nvmlDevice_t device;
        r = nvmlDeviceGetHandleByIndex(i, &device);
        if (r != NVML_SUCCESS) {
            m_log->warn("failed to access device {}: {}", i, r);
            continue;
        }
        // create an instance of this device and check if it is working
        SPDLOG_TRACE(m_log,"creating sensor instance");
        NVidiaSensor *sensor = new NVidiaSensor(device, this);
        if (sensor->isValid())
            m_sensors.push_back(sensor);
        else
            delete sensor;

    }
    return true;
}


void NVidia::run()
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
    connect(m_timer, &QTimer::timeout, this, &NVidia::poll);
       
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
    
    m_log->info("NVidia handler up and running");
}

void NVidia::poll()
{
    SPDLOG_TRACE(m_log, "polling on all devices");
    for (auto it = m_sensors.begin(); it != m_sensors.end(); ++it)
        (*it)->poll();
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
