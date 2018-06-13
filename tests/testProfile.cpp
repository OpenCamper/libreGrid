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

#include "testProfile.h"

#include <QTest>
#include <QDBusArgument>

QTEST_GUILESS_MAIN(testProfile)

inline
libreGrid::Rule createRule(const QString &name, const QString &sensor, const QString &temperature)
{
    libreGrid::Rule rule;
    rule.setName(name);
    rule.setSensor(sensor);
    rule.setTemperature(temperature);
    
    return rule;
}


void testProfile::initTestCase()
{
    // Called before the first testfunction is executed
}

void testProfile::cleanupTestCase()
{
    // Called after the last testfunction was executed
}

void testProfile::init()
{
    // Called before each testfunction is executed
}

void testProfile::cleanup()
{
    // Called after every testfunction
}

void testProfile::rule()
{
    // create a new Rule object
    libreGrid::Rule rule = createRule("myrule", "mysensor", "mytemp");

    rule.addFan("myctl", 0);
    rule.addTemperatureStep(20, 0);
    rule.addTemperatureStep(50, 100);


    // serialize the rule
    QDBusArgument arg;

    arg << rule;

    // TODO send this whole thing via dbus .. jeah, that is getting funny i know ...
}

void testProfile::profile()
{
    // create a new Rule object
    libreGrid::Rule rule1 = createRule("myrule", "mysensor", "mytemp");

    rule1.addFan("myctl", 0);
    rule1.addTemperatureStep(20, 0);
    rule1.addTemperatureStep(50, 100);
    
    // another object
    libreGrid::Rule rule2 = createRule("myrule2", "mysensor2", "mytemp2");

    rule2.addFan("myctl", 1);
    rule2.addTemperatureStep(30, 0);
    rule2.addTemperatureStep(40, 100);
    
    // construct our profile
    libreGrid::Profile profile;
    
    profile.addRule(std::move(rule1));
    profile.addRule(std::move(rule2));

    // serialize the rule
    QDBusArgument arg;
    arg << profile;

    // TODO send this whole thing via dbus .. jeah, that is getting funny i know ...
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
