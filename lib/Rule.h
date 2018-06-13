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

#ifndef LIBREGRID_RULE_H
#define LIBREGRID_RULE_H

#include <QObject>
#include <QUuid>
#include <QDBusArgument>

namespace libreGrid
{
class Rule;
}

/** \addtogroup serializers
 *  @{
 */

QDBusArgument &operator<<(QDBusArgument &argument, const libreGrid::Rule &rule);
const QDBusArgument &operator>>(const QDBusArgument &argument, libreGrid::Rule &rule);

/** @}*/


namespace libreGrid
{

class RulePrivate;

/**
 * Rule for the fan controll profiles.
 *
 */
class Rule
{
public:
    /** @name constructors
     *  @{
     */

    /**
     * Constructor
     *
     * @param parent parent object
     */
    Rule();

    /**
     * copy constructor
     */
    Rule(const Rule &other);

    /**
     * move constructor
     */
    Rule(Rule &&other);

    /** @} */

    /**
     * Destructor
     */
    ~Rule();

    /**
     * @name assignment operators
     * @{
     */

    /**
     * move assignment operator
     */
    Rule &operator=(Rule &&other);

    /**
     * default assignment operator
     */
    Rule &operator=(const Rule &other);

    /** @} */

    /** @name getters
     *  @{
     */

    /**
     * unique identifier of the rule
     *
     * @todo should we keep the identifier here?
     * This would imply that the client has to update it from the server,
     * or we have to implement dbus functions here and connect it to the backend
     */
    const QUuid &identifier() const;

    /**
     * name of the rule
     */
    const QString &name() const;

    /**
     * identifier of the sensor
     */
    const QString &sensor() const;

    /**
     * identifier of the temperature enpoint of the sensor
     */
    const QString &temperature() const;

    /**
     * get the list of fans
     */
    const QVector<QPair<QString, uint> > &fans() const;

    /**
     * get the list if temperature steps
     */
    const QVector<double> &steps() const;

    /**
     * get the list if fan speeds
     *
     * the speed is regulated linear between these values
     * based on the temperature steps
     *
     * the speed values are in percent
     */
    const QVector<uint> &speeds() const;

    /** @}*/

    /** @name setters
     *  @{
     */

    /**
     * set the name of the rule
     */
    void setName(const QString &name);

    /**
     * set the sensor
     *
     * @param sensor identifier of the sensor
     */
    void setSensor(const QString &sensor);

    /**
     * set the temperature enpoint of the sensor
     *
     * @param temperature identifier of the temperature enpoint
     */
    void setTemperature(const QString &temperature);

    /**
     * set the list of fans
     *
     * each element of the list is a QPair,
     * containing the identifier of the fan controller
     * and the id of the fan
     *
     * The list may not contain duplicates or isValid() will fail
     */
    void setFans(const QVector<QPair<QString, uint> > &fans);

    /**
     * add a fan to the fans list
     *
     * @param controller identifier of the controller
     * @param fan id of the fan
     */
    void addFan(const QString &identifier, uint fan);

    /**
     * remove a fan by controller and id
     *
     * @param controller identifier of the controller
     * @param fan id of the fan
     */
    void removeFan(const QString &identifier, uint fan);

    /**
     * clear the list of fans
     */
    void clearFans();

    /**
     * set the list if temperature steps
     *
     * The given list must be ordered, or isValid() will fail.
     */
    void setSteps(const QVector<double> &steps);

    /**
     * set the list if fan speeds
     *
     * the speed is regulated linear between these values
     * based on the temperature steps
     *
     * the speed values are in percent
     */
    void setSpeeds(const QVector<uint> &speeds);


    /**
     * add a temperature/speed step to the list
     *
     * the step is added at the logical correct position
     *
     * @param temperature temperature value
     * @param speed target fan speed in percent
     *
     * @returns the position of the added step
     */
    uint addTemperatureStep(double temperature, uint speed);

    /**
     * remove a temperature/speed step by its position
     *
     * @param pos position within the list
     */
    void removeTemperatureStep(uint pos);

    /**
     * remove a temperature/speed step by its temperature value
     *
     * @param temperature temperature value
     */
    void removeTemperatureStep(double temperature);

    /** @}*/



    /**
     * check if the Rule is valid
     */
    bool isValid() const;


private:
    // we allow our dbus stream operators to access this class directly
    friend QDBusArgument &::operator<<(QDBusArgument &argument, const libreGrid::Rule &rule);
    friend const QDBusArgument &::operator>>(const QDBusArgument &argument, libreGrid::Rule &rule);

    QScopedPointer<RulePrivate> d_ptr;
    Q_DECLARE_PRIVATE(Rule)
};

}

Q_DECLARE_METATYPE(libreGrid::Rule)

#endif // LIBREGRID_RULE_H
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
