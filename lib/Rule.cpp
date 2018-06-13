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

#include <libreGrid/Rule.h>
#include "Rule_p.h"

#include <QDBusMetaType>

using namespace libreGrid;

int RulePrivate::_metaTypeID = -1;


Rule::Rule()
    : d_ptr(new RulePrivate())
{
    // register only once
    if (d_func()->_metaTypeID == -1) {
        d_func()->_metaTypeID = qRegisterMetaType<Rule>("Rule");
        qDBusRegisterMetaType<Rule>();
    }

    // initalize our UUID
    d_func()->m_identifier = QUuid::createUuid();
}


Rule::Rule(const Rule &other)
    : d_ptr(new RulePrivate(*(other.d_func())))
{ }

Rule::Rule(Rule  &&other)
{
    d_ptr.swap(other.d_ptr);
}


Rule::~Rule()
{ }

Rule &Rule::operator=(Rule && other)
{
    d_ptr.swap(other.d_ptr);
    return *this;
}

Rule &Rule::operator=(const Rule &other)
{
    d_ptr.reset(new RulePrivate(*(other.d_func())));
    return *this;
}

const QUuid &libreGrid::Rule::identifier() const
{
    return d_func()->m_identifier;
}

const QString &Rule::name() const
{
    return d_func()->m_name;
}

const QString &Rule::sensor() const
{
    return d_func()->m_sensor;
}

const QString &Rule::temperature() const
{
    return d_func()->m_temperature;
}

const QVector<QPair<QString, uint> > &Rule::fans() const
{
    return d_func()->m_fans;
}

const QVector<double> &Rule::steps() const
{
    return d_func()->m_steps;
}

const QVector<uint> &Rule::speeds() const
{
    return d_func()->m_speeds;
}


void Rule::setName(const QString &name)
{
    d_func()->m_name = name;
}

void Rule::setSensor(const QString &sensor)
{
    d_func()->m_sensor = sensor;
}

void Rule::setTemperature(const QString &temperature)
{
    d_func()->m_temperature = temperature;
}

void Rule::setFans(const QVector<QPair<QString, uint> > &fans)
{
    d_func()->m_fans = fans;

    // now, we need to validate the list on request
    d_func()->m_fansNeedValidation = true;
}

void Rule::setSteps(const QVector<double> &steps)
{
    d_func()->m_steps = steps;
    // now, we need to validate the lists on request
    d_func()->m_tempStepsNeedValidation = true;
}

void Rule::setSpeeds(const QVector<uint> &speeds)
{
    d_func()->m_speeds = speeds;
    // now, we need to validate the lists on request
    d_func()->m_tempStepsNeedValidation = true;
}

void Rule::addFan(const QString &identifier, uint fan)
{
    // check if the fan is already in the list
    for (auto it = d_func()->m_fans.begin(); it != d_func()->m_fans.end(); ++it)
        if (it->first == identifier && it->second == fan)
            return;
    d_func()->m_fans.push_back(QPair<QString, uint>(identifier, fan));
}

void Rule::removeFan(const QString &identifier, uint fan)
{
    for (auto it = d_func()->m_fans.begin(); it != d_func()->m_fans.end(); ++it)
        if (it->first == identifier && it->second == fan)
            d_func()->m_fans.erase(it);
}

void Rule::clearFans()
{
    d_func()->m_fans.clear();
}

uint Rule::addTemperatureStep(double temperature, uint speed)
{
    // first find the correct position within the temperature steps
    auto it = d_func()->m_steps.begin();
    for (; it != d_func()->m_steps.end() ; ++it)
        if ((*it) > temperature)
            break;

    // insert the temperature value into the step list
    it = d_func()->m_steps.insert(it, temperature);

    // calculate the index position of the
    uint pos = it - d_func()->m_steps.begin();

    // insert the speed value
    d_func()->m_speeds.insert(pos, speed);

    return speed;
}

void Rule::removeTemperatureStep(double temperature)
{
    // find the temperature value in the step list
    int pos = d_func()->m_steps.indexOf(temperature);

    if (pos < 0)
        return;

    // remove the values in both vectors
    d_func()->m_steps.remove(pos);
    d_func()->m_speeds.remove(pos);
}

bool Rule::isValid() const
{
    // check if we have all needed string members
    if (d_func()->m_name.isEmpty() || d_func()->m_sensor.isEmpty() || d_func()->m_temperature.isEmpty())
        return false;

    // we need at least one fan
    if (d_func()->m_fans.isEmpty())
        return false;

    // and 2 temperature and speed values
    if (d_func()->m_steps.count() < 2 || d_func()->m_speeds.count() < 2)
        return false;

    if (d_func()->m_fansNeedValidation)
        // we check if there are any duplicates in the fan list
        for (auto it = d_func()->m_fans.begin(); it != d_func()->m_fans.end(); ++it)
            if (d_func()->m_fans.indexOf(*it, (it - d_func()->m_fans.begin()) + 1) != -1)
                return false;

    if (d_func()->m_tempStepsNeedValidation) {
        // we check if the length of both vectors are the same
        if (d_func()->m_steps.count() != d_func()->m_speeds.count())
            return false;

        // we check the steps list is sorted
        for (auto it = d_func()->m_steps.begin() + 1; it != d_func()->m_steps.end(); ++it)
            if ((*it) <= (*(it - 1)))
                return false;
    }

    return true;
}


// these are our serializers
QDBusArgument &operator<<(QDBusArgument &argument, const libreGrid::Rule &rule)
{
    const libreGrid::RulePrivate *p = rule.d_func();

    argument.beginStructure();
    // strings first, they are simple
    argument << p->m_identifier.toByteArray() << p->m_name << p->m_sensor << p->m_temperature;

    // next the vectors
    // TODO we could use the array handling for this, but then we need to add
    // marshaling functions for the QPair. This way is no such elegent, mut more effective for us atm

    // submit the size of the list
    argument << p->m_fans.count();
    for (auto it = p->m_fans.begin(); it != p->m_fans.end(); ++it) {
        argument.beginStructure();
        argument << it->first << it->second;
        argument.endStructure();
    }

    // submit the last two vectors, they are straight forward
    argument << p->m_steps << p->m_speeds;

    argument.endStructure();
    return argument;

}

const QDBusArgument &operator>>(const QDBusArgument &argument, libreGrid::Rule &rule)
{
    libreGrid::RulePrivate *p = rule.d_func();
    argument.beginStructure();

    // restore our strings
    QByteArray uuid;
    argument >> uuid;
    p->m_identifier = QUuid(uuid);
    argument >> p->m_name >> p->m_sensor >> p->m_temperature;

    // restore the size of the following vector
    uint vector_size;

    argument >> vector_size;
    for (uint i = 0; i < vector_size; ++i) {
        QString ctrl;
        uint fan;

        argument.beginStructure();
        argument >> ctrl >> fan;
        p->m_fans.push_back(QPair<QString, uint>(ctrl, fan));
        argument.endStructure();
    }

    // submit the last two vectors, they are straight forward
    argument >> p->m_steps >> p->m_speeds;

    argument.endStructure();
    return argument;
}



// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
