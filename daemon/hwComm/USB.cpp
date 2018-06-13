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

#include <stdexcept>
#include <QMetaObject>
#include <QDebug>
#include <QCoreApplication>

#include "USB.h"
#include "GRIDv3.h"
#include "usb_macros.h"


// we need to register the meta types for callback handling
Q_DECLARE_METATYPE(libusb_hotplug_event)

// we need a static function for the callback handling, two wrapp around our class
static int LIBUSB_CALL hotplug_callback(libusb_context *ctx, libusb_device *dev, libusb_hotplug_event event, void *user_data)
{
    // we don't care about the context, we only have one, stored in our class
    Q_UNUSED(ctx)

    // we simply invoke the slot of the USB class
    libreGrid::server::HW::USB *instance = (libreGrid::server::HW::USB *)user_data;
    QMetaObject::invokeMethod(instance, "usb_hotplug_event", Q_ARG(void *, dev),
                              Q_ARG(libusb_hotplug_event, event));

    return 0;
}

using namespace libreGrid::server::HW;

USB::USB()
{
    // get our logger
    m_log = spdlog::get("USB");

    SPDLOG_TRACE(m_log, "registering USB metatypes");
    qRegisterMetaType<libusb_hotplug_event>("libusb_hotplug_event");

    SPDLOG_TRACE(m_log, "USB interface constructed");
}

USB::~USB()
{
    // close all controllers
    SPDLOG_TRACE(m_log, "destructing all devices");
    for (auto it = m_devices.begin(); it != m_devices.end() ; ++it)
        delete(*it);

    SPDLOG_TRACE(m_log, "USB interface destroyed");
}

void USB::init_usb()
{
    SPDLOG_TRACE(m_log, "initalizing libusb");
    // initialize the default USB context
    CHECK_OR_THROW(libusb_init(&m_usb_ctx), "failed to initialize USB context")

    // we register a callback and arm it, to get all devices and get notified about new devices in the future
    // currently, we match only against GRIDv3+. If we support more devices, we should listen to LIBUSB_HOTPLUG_MATCH_ANY
    SPDLOG_TRACE(m_log, "registering hotplug callback");
    CHECK_OR_THROW(
        libusb_hotplug_register_callback(
            m_usb_ctx,
            (libusb_hotplug_event)(LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED | LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT),
            LIBUSB_HOTPLUG_ENUMERATE,
            0x1e71, 0x1711,
            LIBUSB_HOTPLUG_MATCH_ANY,
            &hotplug_callback,
            (void *)this,
            nullptr
        ),
        "failed to register hotplug callback"
    )
    // at this point, libusb should fire the callback function each time it finds a new fancontroller device, we didn't know yet
}

void USB::usb_hotplug_event(void *device, libusb_hotplug_event event)
{
    libusb_device *d = (libusb_device *)device;
    SPDLOG_TRACE(
        m_log, "received hotplug event {}",
        event, QThread::currentThreadId(), m_thread->currentThreadId()
    );

    // if a new device arrived, we add it to a queue and process this outside the current
    // event context
    if (event == LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED) {
        SPDLOG_TRACE(m_log, "adding device handler to the hotplug queue");
        m_hotplug_queue.enqueue(d);
    } else if (event == LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT)
        this->remove_device(d);
    // we don't care about any other, dubios event ...
}


void USB::add_device(libusb_device *device)
{
    SPDLOG_TRACE(m_log, "device hotplug requested");
    // check if the device exists
    if (m_devices.find(device) != m_devices.end()) {
        SPDLOG_TRACE(m_log, "the device does already exist");
        return;
    }

    // create our fanController instance
    SPDLOG_TRACE(m_log, "creating a new controller for the device");
    GRIDv3 *controller = new GRIDv3(this, device);

    // check if the fanController is valid
    if (!controller->isValid()) {
        m_log->error("failed to add FanController: {}", controller->lastError().toStdString());
        SPDLOG_TRACE(m_log, "deleting controller");
        delete controller;
        return;
    }

    // add our device to our map
    m_devices[device] = controller;

    // send a signal that we got a new device
    SPDLOG_TRACE(m_log, "emitting hotplug signal");
    emit device_plugged(controller->identifier(), controller);
    m_log->info("new GRIDv3 device connected: {}", controller->identifier().toStdString());
}


void USB::remove_device(libusb_device *device)
{
    SPDLOG_TRACE(m_log, "device hotremove requested");
    // search for the device
    auto it = m_devices.find(device);

    // end here if we don't have the device in our list
    if (it == m_devices.end()) {
        SPDLOG_TRACE(m_log, "device is unknown to us");
        return;
    }

    // emit the signal that we are removing the device
    SPDLOG_TRACE(m_log, "emitting hotremove signal");
    emit device_unplugged((*it)->identifier());

    m_log->info("GRIDv3 device removed: {}", (*it)->identifier().toStdString());

    // delete the device
    SPDLOG_TRACE(m_log, "destroying GRIDv3 interface");
    delete(*it);

    // remove the dead device from the list
    m_devices.erase(it);
}

void USB::run()
{
    SPDLOG_TRACE(m_log, "creating new thread");
    m_thread = new QThread();

    SPDLOG_TRACE(m_log, "moving this instance to thread");
    moveToThread(m_thread);

    SPDLOG_TRACE(m_log, "connecting thread signals");
    connect(m_thread, &QThread::started, this, &USB::loop);

    // make sure we clean up the thread after all is done
    connect(m_thread, &QThread::finished, m_thread, &QThread::deleteLater);
    connect(m_thread, &QThread::finished, this,     &QThread::deleteLater);
    
    
    SPDLOG_TRACE(m_log, "starting thread");
    m_thread->start();
}

void USB::loop()
{
    m_log->info("USB handler up and running");
    SPDLOG_TRACE(m_log, "initalizing usb stack");
    init_usb();

    m_log->info("entering main loop");
    struct timeval tv = {0 , 0};
    while (1) {
        {
            QMutexLocker locker(&m_stop_mutex);
            if (m_stopRequested) {
                SPDLOG_DEBUG(m_log, "thread shutdown requested");
                m_thread->quit();
                return;
            }
        }

        // process pending usb events
        libusb_handle_events_timeout_completed(0, &tv, 0);

        // process our event list
        QCoreApplication::processEvents();

        // process our hotplug queue
        process_hotplug();

        // iterate over our controllers
        for (auto it = m_devices.begin(); it != m_devices.end(); ++it) {
            // handle the current controller
            if (!(*it)->process_events()) {
                SPDLOG_DEBUG(
                    m_log, "failed to process events for device {}: {}",
                    (*it)->identifier().toStdString(), (*it)->lastError().toStdString()
                );
            }
        }
    }
    m_log->info("USB handler shut down");
}

void USB::process_hotplug()
{
    //SPDLOG_TRACE(m_log, "processing hotplug queue");
    while (!m_hotplug_queue.isEmpty())
        add_device(m_hotplug_queue.dequeue());
}

void USB::quit()
{
    QMutexLocker locker(&m_stop_mutex);
    m_stopRequested = true;
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
