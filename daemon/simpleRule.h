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

#ifndef LIBREGRID_SERVER_SIMPLERULE_H
#define LIBREGRID_SERVER_SIMPLERULE_H

#include <QObject>
#include <QVector>
#include <libreGrid/types.h>
#include <QUuid>

#include "Settings.h"

namespace libreGrid
{
namespace server
{

/**
 * @todo write docs
 */
class simpleRule : public QObject
{
    Q_OBJECT

    Q_CLASSINFO("D-Bus Interface", "libre.Grid1.profile.simpleRule")

    Q_PROPERTY(libreGrid::FanList fans READ fans WRITE setFans)
    Q_PROPERTY(QUuid identifier READ identifier CONSTANT)
    Q_PROPERTY(QList<double> limits READ limits WRITE setLimits)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString sensor READ sensor WRITE setSensor)
    Q_PROPERTY(QList<uint> speeds READ speeds WRITE setSpeeds)
    Q_PROPERTY(QString temperature READ temperature WRITE setTemperature)

public:
    /**
     * Destructor
     */
    ~simpleRule();

    /**
     * Constructor
     *
     * @param parent TODO
     */
    simpleRule(QObject *parent, QUuid identifier = QUuid());

    QUuid identifier() const;
    
    const libreGrid::FanList fans();
    void setFans(const libreGrid::FanList &value);

    QList < double > limits() const;
    void setLimits(const QList < double > &value);

    QString name() const;
    void setName(const QString &value);

    QString sensor() const;
    void setSensor(const QString &value);

    QList < uint > speeds() const;
    void setSpeeds(const QList<uint> &value);

    QString temperature() const;
    void setTemperature(const QString &value);

signals:
    void fansChanged(const libreGrid::FanList &fans);
    void limitsChanged(const QList<double> &limits);
    void nameChanged(const QString &name);
    void sensorChanged(const QString &name);
    void speedsChanged(const QList<uint> &speeds);
    void temperatureChanged(const QString &temperature);

public slots:
    bool addFan(const QString &controller, uint fan);
    bool remFan(const QString &controller, uint fan);

private:
    // our settings object
    QSettings *m_settings;
    QUuid   m_identifier;
    FanList m_fans;
};

}
}

#endif // LIBREGRID_SERVER_SIMPLERULE_H
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
