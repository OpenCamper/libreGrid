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

#ifndef LIBREGRID_PROFILE_P_H
#define LIBREGRID_PROFILE_P_H

#include <QString>
#include <QUuid>

namespace libreGrid
{

/**
 * @todo write docs
 */
class ProfilePrivate
{
    friend class Profile;

    friend QDBusArgument &::operator<<(QDBusArgument &argument, const libreGrid::Profile &profile);
    friend const QDBusArgument &::operator>>(const QDBusArgument &argument, libreGrid::Profile &profile);

    // identifier
    QUuid       m_identifier;

    // name
    QString     m_name;

    // list of rules
    QMap<QUuid, Rule> m_rules;
    
    // our type id
    static int _metaTypeID;
};

}

#endif // LIBREGRID_PROFILE_P_H
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
