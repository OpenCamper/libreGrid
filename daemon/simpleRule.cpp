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

#include "simpleRule.h"
#include "simpleruleadaptor.h"

using namespace libreGrid::server;

// register our types we only use here
Q_DECLARE_METATYPE(QList<double>)
Q_DECLARE_METATYPE(QList<uint>)

simpleRule::simpleRule(QObject *parent, QUuid identifier)
    : QObject(parent), m_identifier(identifier)
{
    // create the adaptor
    (void)new SimpleRuleAdaptor(this);

    if (m_identifier.isNull())
        m_identifier = QUuid::createUuid();

    // initalize our settings object
    m_settings = Settings::get(this);
    m_settings->beginGroup("Rules");
    m_settings->beginGroup(m_identifier.toString());
}

simpleRule::~simpleRule()
{ }

QUuid simpleRule::identifier() const
{
    return this->m_identifier;
}

const libreGrid::FanList simpleRule::fans()
{
    QVariant v = m_settings->value("fans");
    if (v.isValid()) {
        m_fans = v.value<libreGrid::FanList>();
    }
    return m_fans;
}

void simpleRule::setFans(const libreGrid::FanList &value)
{
    m_settings->setValue("fans", QVariant::fromValue(value));
    m_fans = value;
    emit fansChanged(value);
}

QList<double> simpleRule::limits() const
{
    QVariant v = m_settings->value("limits");
    return v.isValid() ? v.value<QList<double> >() : QList<double>();
}

void simpleRule::setLimits(const QList<double> &value)
{
    m_settings->setValue("limits", QVariant::fromValue(value));
    emit limitsChanged(value);
}

QString simpleRule::name() const
{
    return m_settings->value("name", m_identifier).toString();
}

void simpleRule::setName(const QString &value)
{
    m_settings->setValue("name", value);
    emit nameChanged(value);
}

QString simpleRule::sensor() const
{
    return m_settings->value("sensor").toString();
}

void simpleRule::setSensor(const QString &value)
{
    m_settings->setValue("sensor", value);
    emit sensorChanged(value);
}

QList<uint> simpleRule::speeds() const
{
    QVariant v = m_settings->value("speeds");
    return v.isValid() ? v.value<QList<uint> >() : QList<uint>();
}

void simpleRule::setSpeeds(const QList<uint> &value)
{
    m_settings->setValue("speeds", QVariant::fromValue(value));
    emit speedsChanged(value);
}

QString simpleRule::temperature() const
{
    return m_settings->value("temperature").toString();
}

void simpleRule::setTemperature(const QString &value)
{
    m_settings->setValue("temperature", value);
    emit temperatureChanged(value);
}


bool simpleRule::addFan(const QString &controller, uint fan)
{
    // check if we have a duplicate
    libreGrid::FanIdentifier fanID(controller, fan);
    if (m_fans.find(fanID) != m_fans.end())
        return false;

    m_fans.insert(fanID);
    setFans(m_fans);
    return true;
}

bool simpleRule::remFan(const QString &controller, uint fan)
{
    libreGrid::FanIdentifier fanID(controller, fan);
    bool ret = m_fans.remove(fanID);
    setFans(m_fans);
    return ret;
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
