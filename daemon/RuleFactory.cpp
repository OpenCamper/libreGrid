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

#include "RuleFactory.h"

using namespace libreGrid::server;

// initalize our static members
RuleFactory *RuleFactory::_instance   = nullptr;
QMutex      *RuleFactory::_instance_m = new QMutex();

RuleFactory *RuleFactory::instance()
{
    {
        QMutexLocker locker(_instance_m);
        if (_instance == nullptr)
            _instance = new RuleFactory();
    }

    return _instance;
}

RuleFactory::RuleFactory()
    : QObject(nullptr)
{
    m_settings = Settings::get(this);
    m_settings->beginGroup("Rules");

    // we create every known rule object
    QStringList rulelist = m_settings->childGroups();

    for (auto it = rulelist.begin(); it != rulelist.end(); ++it)
        m_rules[*it] = new simpleRule(this, QUuid(*it));

    m_rules_mutex = new QMutex();
}

simpleRule *RuleFactory::getRule(QString identifier)
{
    // check if the rule exists
    QMutexLocker locker(m_rules_mutex);
    if (m_rules.contains(identifier))
        return m_rules[identifier];
    else
        return nullptr;
}

simpleRule *RuleFactory::getRule(QUuid identifier)
{
    return getRule(identifier.toString());
}


simpleRule * RuleFactory::createRule()
{
    // create the new rule
    simpleRule *rule = new simpleRule(this);
    
    QMutexLocker locker(m_rules_mutex);
    m_rules[rule->identifier().toString()] = rule;
    
    return rule;
}


RuleFactory::~RuleFactory()
{
    delete m_rules_mutex;
}



// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
