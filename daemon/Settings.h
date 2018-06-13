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

#ifndef LIBREGRID_SERVER_SETTINGS_H
#define LIBREGRID_SERVER_SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QMutex>

namespace libreGrid
{

namespace server
{

class Settings
{


public:
    static void init(const QString filename);
    static QSettings* get(QObject *parent = nullptr);

private:
    Settings(const QString &path);
    static Settings *_instance;
    static QMutex   *_m;

    QString           m_path;
    QSettings::Format m_format;
};

}
}

#endif // LIBREGRID_SETTINGS_H
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
