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

#ifndef LIBREGRID_RULE_P_H
#define LIBREGRID_RULE_P_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QUuid>

namespace libreGrid
{

/**
 * @todo write docs
 */
class RulePrivate
{
    friend class Rule;
    
    // we allow our dbus stream operators to access this class directly
    friend QDBusArgument &::operator<<(QDBusArgument &argument, const libreGrid::Rule &rule);
    friend const QDBusArgument &::operator>>(const QDBusArgument &argument, libreGrid::Rule &rule);
    
    // our unique identifier
    QUuid m_identifier;
    
    // our name
    QString m_name;
    
    // our input sensor chip
    QString m_sensor;

    // our input sensor temperatur
    QString m_temperature;

    // the list of fans, this rule controlls
    // we use a QPair to combine controller identifier and fan id
    QVector<QPair<QString, uint> > m_fans;
    // we keep a flag to check if we need to validate the vector
    bool m_fansNeedValidation = false;
    
    
    // list of temperature steps
    QVector<double> m_steps;   
    // speeds at the given points
    QVector<uint>   m_speeds;
    // we keep a flag to check if we need to validate the vectors
    bool m_tempStepsNeedValidation = false;
    
    static int _metaTypeID;
    
};

}

#endif // LIBREGRID_RULE_P_H
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
