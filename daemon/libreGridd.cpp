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

#include <QCoreApplication>
#include <QDBusMetaType>
#include <csignal>

#include "Controller.h"
#include "Settings.h"

#include <QDir>
#include <QSettings>


struct SignalHandler {
    SignalHandler()
    {
        std::signal(SIGINT, &SignalHandler::exitQt);
    }

    static void exitQt(int sig)
    {
        Q_UNUSED(sig)
        QCoreApplication::exit(0);
    }
};

int main(int argc, char *argv[])
{

    SignalHandler signalHandler;

    QCoreApplication a(argc, argv);

    QDBusConnection dbus = QDBusConnection::systemBus();
    
    // prepare our settings
    libreGrid::server::Settings::init("/tmp/grid.json");
    
    // instanciate our central controller
    libreGrid::server::Controller controller(dbus);
    
    // setup the controller and connect all signals
    controller.run();
    
    // run the mainloop
    return a.exec();
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 

