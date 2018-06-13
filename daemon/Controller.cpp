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

#include <iostream>
#include <QSettings>

#include "hwComm/lmSensorsChip.h"
#include "Controller.h"

#include "controlleradaptor.h"

using namespace libreGrid::server;

Controller::Controller(QDBusConnection &dbus, QObject *parent)
    : QObject(parent), m_dbus(dbus)
{
    // init the loggers
    initLog();

    // intialize the controller adaptor
    SPDLOG_TRACE(m_log, "initializing DBus adaptor");
    (void)new ControllerAdaptor(this);

    // initalize our settings object
    SPDLOG_TRACE(m_log, "initializing settings object");
    m_settings = Settings::get(this);

    // create the USB controller thread object
    SPDLOG_TRACE(m_log, "creating the USB worker thread");
    m_usb_controller = new HW::USB();

    // create the lm_sensors thread
    SPDLOG_TRACE(m_log, "creating the lmSensors worker thread");
    m_lmsensors = new HW::lmSensors();

#ifdef HAS_NVML
    // create the nvidia thread
    SPDLOG_TRACE(m_log, "creating the NVidia worker thread");
    m_nvidia = new HW::NVidia();
#endif

    // we register ourself on the bus
    SPDLOG_TRACE(m_log, "registering DBus service");
    m_dbus.registerService("libre.Grid1.controller");
    SPDLOG_TRACE(m_log, "registering DBus object");
    m_dbus.registerObject("/libre/Grid1/controller", this);
}


Controller::~Controller()
{

    // disable the hotplug events for usb
    SPDLOG_TRACE(m_log, "disconnecting USB hotplug signal");
    disconnect(m_usb_controller, &HW::USB::device_plugged, this, &Controller::usb_device_plugged);

    // remove our fanController instances
    SPDLOG_TRACE(m_log, "remove all Devices DBUS interfaces");
    for (auto it = m_devices.begin(); it != m_devices.end(); ++it)
        delete *it;

    // remove our sensor instances
    SPDLOG_TRACE(m_log, "remove all Sensors DBUS interfaces");
    for (auto it = m_sensors.begin(); it != m_sensors.end(); ++it)
        delete *it;

    // wait for our usb controller to shut down
    SPDLOG_TRACE(m_log, "requesting stop for the USB worker thread");
    m_usb_controller->quit();
    SPDLOG_TRACE(m_log, "waiting for the USB worker to shut down");
    m_usb_controller->wait();

    if (m_lmsensors != nullptr) {
        // wait for the lm_sensors thread to clean up
        SPDLOG_TRACE(m_log, "requesting stop for the lmSensors worker thread");
        m_lmsensors->quit();
        SPDLOG_TRACE(m_log, "waiting for the lmSensors worker to shut down");
        m_lmsensors->wait();
    }

#ifdef HAS_NVML
    if (m_nvidia != nullptr) {
        // wait for the nvidia thread to clean up
        SPDLOG_TRACE(m_log, "requesting stop for the NVidia worker thread");
        m_nvidia->quit();
        SPDLOG_TRACE(m_log, "waiting for the NVidia worker to shut down");
        m_nvidia->wait();
    }
#endif

    // clean up the dbus
    SPDLOG_TRACE(m_log, "unregistering DBus object");
    m_dbus.unregisterObject("/libre/Grid1/controller");
    SPDLOG_TRACE(m_log, "unregistering DBus service");
    m_dbus.unregisterService("libre.Grid1.controller");


}

QStringList Controller::availibleDevices() const
{
    QStringList devices;

    // append our devices
    for (auto it = m_devices.begin(); it != m_devices.end(); ++it)
        devices << it.key();

    return devices;
}

QStringList Controller::availibleSensors() const
{
    QStringList sensors;

    // append our devices
    for (auto it = m_sensors.begin(); it != m_sensors.end(); ++it)
        sensors << it.key();

    return sensors;
}

QString Controller::activeProfile() const
{
    return m_settings->value("profile", QString()).toString();
}


bool Controller::enabled() const
{
    return m_settings->value("enabled", true).toBool();
}


void Controller::setEnabled(bool enabled)
{
    m_settings->setValue("enabled", enabled);
    // TODO stub!
    // we have to make sure that no processing is done if the controller is set to disabled!
}

void Controller::setActiveProfile(const QString &profile)
{
    m_settings->setValue("profile", profile);
}


QString Controller::createProfile()
{
    Profile *profile = new Profile(m_dbus, this);
    m_profiles[profile->identifier().toString()] = profile;
    return profile->identifier().toString();
}


void Controller::deleteProfile(const QString &identifier)
{
    // delete the profile
    m_profiles[identifier]->deleteProfile();

    // cleanup our list
    m_profiles.remove(identifier);
}


void Controller::run()
{
    // connect the signals of the USB controller
    SPDLOG_TRACE(m_log, "connecting the USB hotplug signal");
    connect(m_usb_controller, &HW::USB::device_plugged, this, &Controller::usb_device_plugged);
    SPDLOG_TRACE(m_log, "connecting the USB hot-unplug signal");
    connect(m_usb_controller, &HW::USB::device_unplugged, this, &Controller::usb_device_unplugged);

    // and finaly start the thread
    SPDLOG_TRACE(m_log, "starting USB work thread");
    m_usb_controller->run();

    // init the lm_sensors handler
    SPDLOG_TRACE(m_log, "initializing lmSensors stack");
    if (m_lmsensors->init()) {
        SPDLOG_TRACE(m_log, "getting list of lmSensor chips");
        QList<HW::lmSensorsChip *> lmchips = m_lmsensors->chips();
        // create an instance for every chip
        SPDLOG_TRACE(m_log, "creating instances for all lmSensor Chips");
        for (auto it = lmchips.begin(); it != lmchips.end(); ++it)
            m_sensors[(*it)->identifier()] = new Sensor(m_dbus, *it, this);

        // start the lm_sensors worker
        SPDLOG_TRACE(m_log, "starting lmSensors worker thread");
        m_lmsensors->run();
    } else {
        m_log->warn("could not initalize lmSensors");
        // destroy the lm_sensors handler
        SPDLOG_TRACE(m_log, "destroying the lmSensors instance");
        delete m_lmsensors;
        m_lmsensors = nullptr;
    }

#ifdef HAS_NVML
    // init the nvidia handler
    SPDLOG_TRACE(m_log, "initializing NVidia stack");
    if (m_nvidia->init()) {
        SPDLOG_TRACE(m_log, "getting list of NVidia sensors");
        QList<HW::NVidiaSensor *> nvsensors = m_nvidia->sensors();
        // create an instance for every chip
        SPDLOG_TRACE(m_log, "creating instances for all NVidia sensors");
        for (auto it = nvsensors.begin(); it != nvsensors.end(); ++it)
            m_sensors[(*it)->identifier()] = new Sensor(m_dbus, *it, this);

        // start the lm_sensors worker
        SPDLOG_TRACE(m_log, "starting NVidia worker thread");
        m_nvidia->run();
    } else {
        m_log->warn("could not initalize NVidia");
        // destroy the lm_sensors handler
        SPDLOG_TRACE(m_log, "destroying the starting instance");
        delete m_nvidia;
        m_nvidia = nullptr;
    }
#endif

    // we need to load our rules and profiles
    
    // TODO load all profiles
    // TODO load all rules
    
    m_log->info("controller up and running");
}


void Controller::usb_device_plugged(const QString &identifier, HW::FanController *controller)
{
    SPDLOG_TRACE(m_log, "received an USB hotplug event for: '{}'", identifier.toStdString());
    // check of we have seend this device already
    if (m_devices.find(identifier) != m_devices.end()) {
        SPDLOG_TRACE(m_log, "device already exists", identifier.toStdString());
        return;
    }

    // create a new controller instance
    SPDLOG_TRACE(m_log, "creating interface for device '{}'", identifier.toStdString());
    Device *dev = new Device(m_dbus, controller, this);

    // add the device to our devices map
    m_devices[identifier] = dev;

    SPDLOG_TRACE(m_log, "emitting hotplug signal for '{}'", identifier.toStdString());
    // signal the listeners about the new device
    emit devicePlugged(identifier);
}

void Controller::usb_device_unplugged(const QString &identifier)
{
    SPDLOG_TRACE(m_log, "received an USB hot-remove event for: '{}'", identifier.toStdString());
    // find the device in our list
    auto it = m_devices.find(identifier);
    if (it == m_devices.end()) {
        SPDLOG_TRACE(m_log, "device is unknown", identifier.toStdString());
        return;
    }

    // notify about the device removal
    SPDLOG_TRACE(m_log, "emitting hotremove signal for '{}'", identifier.toStdString());
    emit deviceUnplugged(identifier);

    // destruct the device class
    SPDLOG_TRACE(m_log, "destroying interface for device '{}'", identifier.toStdString());
    delete(*it);

    // clean our map
    m_devices.erase(it);
}


void Controller::initLog()
{
    // TODO: we should make these things configurable
    spdlog::set_level(spdlog::level::trace);
    // create our default sink
    auto sink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();

    // create our own logger, now we could start
    m_log = spdlog::stdout_color_mt("controller");

#define ADD_LOGGER(name) spdlog::stdout_color_mt(#name);
    ADD_LOGGER(USB)
    ADD_LOGGER(GRIDv3)
    ADD_LOGGER(GRIDv3Fan)
    ADD_LOGGER(lmSensors)
    ADD_LOGGER(lmSensorsChip)
    ADD_LOGGER(NVidia)
#undef ADD_LOGGER

}


// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
