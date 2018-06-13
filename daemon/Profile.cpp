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

#include "Profile.h"
#include "profileadaptor.h"
#include "RuleFactory.h"

using namespace libreGrid::server;

Profile::Profile(QDBusConnection &dbus, QObject *parent, QUuid uuid)
    : QObject(parent), m_identifier(uuid), m_dbus(dbus)
{
    // create the adaptor
    (void)new ProfileAdaptor(this);

    if (m_identifier.isNull())
        m_identifier = QUuid::createUuid();

    // initalize our settings object
    m_settings = Settings::get(this);
    m_settings->beginGroup("Profiles");
    m_settings->beginGroup(m_identifier.toString());

    // register ourself on the dbus
    m_dbusPath = QString("/libre/Grid1/profiles/%1").arg(m_identifier.toString());
    m_dbus.registerObject(m_dbusPath, this);
}


Profile::~Profile()
{
    // clean up the dbus
    m_dbus.unregisterObject(m_dbusPath);
}

const QUuid &Profile::identifier() const
{
    return m_identifier;
}

QString Profile::name() const
{
    return m_settings->value("name", m_identifier.toString()).toString();
}

QStringList Profile::rules() const
{
    // we read the rules from the configuration file
    return m_settings->value("rules", QStringList()).toStringList();
}

bool Profile::addRule(const QString &rule)
{
    RuleFactory *factory = RuleFactory::instance();

    // check if the rule exists
    simpleRule *ruleObj = factory->getRule(rule);
    if (ruleObj == nullptr) return false;

    // we need to check all rules if we controll the same fan from multiple rules
    QStringList curRules = rules();

    for (auto it = curRules.begin(); it != curRules.end(); ++it)
        if (ruleObj->fans().intersects(factory->getRule(*it)->fans()))
            return false;

    // if we reached this point, it is save to add the rule to this profile
    curRules << rule;
    m_settings->setValue("rules", curRules);

    return true;
}


void Profile::remRule(const QString &rule)
{
    QStringList curRules = rules();
    curRules.removeAll(rule);

    m_settings->setValue("rules", rule);
}



bool Profile::valid() const
{
    // TODO stub
    return true;
}

void Profile::setName(const QString &name)
{
    m_settings->setValue("name", name);
    emit nameChanged(name);
}


void Profile::deleteProfile()
{
    // we leave the current settings group
    m_settings->endGroup();

    // and delete ourself
    m_settings->remove(m_identifier.toString());

    // and now, we commit suicide
    delete this;
}


// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
