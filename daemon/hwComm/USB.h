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

#ifndef LIBREGRID_SERVER_HW_USB_H
#define LIBREGRID_SERVER_HW_USB_H

#include <QMap>
#include <QQueue>
#include <QThread>
#include <QMutex>
#include "FanController.h"
#include <spdlog/spdlog.h>
#include "libusbWrapper.h"

namespace libreGrid
{
namespace server
{
namespace HW
{

/**
 * @todo write docs
 */
class USB : public QObject
{
    Q_OBJECT

public:
    /**
     * Default constructor
     */
    USB();

    /**
     * Destructor
     */
    ~USB();


    /**
     * initialize the USB stack
     *
     * this function must be called before any other
     * member can be used.
     *
     * @throws runtime_error if anything fails
     */
    void init_usb();

    /**
     * our own mainloop, called from the thread
     */
    void run();

    /**
     * quit the thread
     */
    void quit();

    /*
     * relay the wait event of the thread
     */
    bool wait()
    {
        return m_thread->wait();
    }

public slots:
    /**
     * handle an hotplug event
     *
     * this slot is only used by the callback machanism of libusb
     */
    void usb_hotplug_event(void *device, libusb_hotplug_event event);

signals:
    /**
     * notifies about a new device
     *
     * @param serial serial of the controller
     */
    void device_plugged(const QString &identifier, FanController *controller);

    /**
     * notifies about a removed device
     *
     * @param serial serial of the controller
     *
     * we can not returen a reference to the string here,
     * as we delete the object containing the key after we emit the signal
     */
    void device_unplugged(QString identifer);


private slots:
    /**
     * this is our event loop
     */
    void loop();

private:
    // handle new devices
    void add_device(libusb_device *device);
    void remove_device(libusb_device *device);
    
    // process the hotplug queue
    void process_hotplug();

    // this is a queue for unhandled hotplug events
    // we need this because we can not call descriptor functions
    // within the hotplug context
    QQueue<libusb_device*> m_hotplug_queue;

    // device list
    QMap<libusb_device *, FanController *> m_devices;

    // speed map ( as we don't know the speeds from the device itself, we have to keep them here)
    QMap<QString, QVector<uint>> m_device_speeds;

    // we use an own usb libusb_context
    libusb_context *m_usb_ctx;


    // handlers for start and stop
    QMutex m_stop_mutex;
    bool m_stopRequested = false;

    // our log instance
    std::shared_ptr<spdlog::logger> m_log;

    // our thread
    QThread     *m_thread;
};


}
}
}

#endif // LIBREGRID_SERVER_HW_USB_H
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
