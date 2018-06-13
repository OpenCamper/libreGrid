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

#include <libreGrid/Profile.h>
#include "Profile_p.h"
#include <QDBusMetaType>

using namespace libreGrid;
int ProfilePrivate::_metaTypeID = -1;

Profile::Profile()
    : d_ptr(new ProfilePrivate())
{
    // init our metatypes only once
    if (d_func()->_metaTypeID == -1) {
        d_func()->_metaTypeID = qRegisterMetaType<Profile>("Profile");
        qDBusRegisterMetaType<Profile>();
    }
    
    // initalize our UUID
    d_func()->m_identifier = QUuid::createUuid();
}

Profile::Profile(const Profile &other)
    : d_ptr(new ProfilePrivate(*(other.d_func())))
{ }


Profile::Profile(Profile  &&other)
{
    d_ptr.swap(other.d_ptr);
}


Profile &Profile::operator=(Profile && other)
{
    d_ptr.swap(other.d_ptr);
    return *this;
}


Profile::~Profile()
{ }

const QUuid & Profile::identifier() const
{
    return d_func()->m_identifier;
}


const QString & Profile::name() const
{
    return d_func()->m_name;
}


bool Profile::addRule(const Rule &rule)
{
    if (d_func()->m_rules.find(rule.identifier()) != d_func()->m_rules.end())
        return false;

    d_func()->m_rules[rule.identifier()] = rule;

    return true;
}

bool Profile::addRule(Rule &&rule)
{
    if (d_func()->m_rules.find(rule.identifier()) != d_func()->m_rules.end())
        return false;

    d_func()->m_rules[rule.identifier()] = rule;
    return true;
}

// serializers

QDBusArgument &operator<<(QDBusArgument &argument, const libreGrid::Profile &profile)
{
    argument.beginStructure();

    // add our string members
    argument << profile.d_func()->m_identifier.toByteArray() << profile.d_func()->m_name;

    // add our map
    //argument << profile.d_func()->m_rules;

    argument.endStructure();

    return argument;
}


const QDBusArgument &operator>>(const QDBusArgument &argument, libreGrid::Profile &profile)
{
    argument.beginStructure();

    QByteArray uuid;
    // get our string members
    argument >> uuid;
    profile.d_func()->m_identifier = QUuid(uuid);
    argument >> profile.d_func()->m_name;

    // get our map
    //argument >> profile.d_func()->m_rules;

    argument.endStructure();

    return argument;
}



// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
