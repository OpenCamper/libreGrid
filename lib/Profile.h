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

#ifndef LIBREGRID_PROFILE_H
#define LIBREGRID_PROFILE_H

#include <QObject>
#include "Rule.h"

namespace libreGrid
{
class Profile;
}

/** \addtogroup serializers
 *  @{
 */

QDBusArgument &operator<<(QDBusArgument &argument, const libreGrid::Profile &profile);
const QDBusArgument &operator>>(const QDBusArgument &argument, libreGrid::Profile &profile);

/** @}*/

namespace libreGrid
{

class ProfilePrivate;

/**
 * @todo write docs
 */
class Profile
{
public:
    /** @name constructors
     *  @{
     */

    /**
     * constructor
     */
    Profile();

    /**
     * copy constructor
     */
    Profile(const Profile &other);

    /**
     * move constructor
     */
    Profile(Profile &&other);

    /** @} */

    /**
     * destructor
     */
    ~Profile();

    /**
     * move assignment operator
     */
    Profile &operator=(Profile && other);

    /** @name getters
     *  @{
     */
    
    /**
     * unique identifier of the profile
     *
     * @todo should we keep the identifier here?
     * This would imply that the client has to update it from the server,
     * or we have to implement dbus functions here and connect it to the backend
     */
    const QUuid &identifier() const;

    /**
     * name of the profile
     */
    const QString &name() const;
    
    /** @} */
    
    /** @name setters
     *  @{
     */
    
    /**
     * set the name of the profile
     */
    void setName(const QString &name);

    
    /**
     * add a new rule
     *
     * @returns false if a rule with the same identifier exists
     */
    bool addRule(const Rule &rule);

    /**
     * add a new rule
     *
     * @returns false if a rule with the same identifier exists
     */
    bool addRule(Rule &&rule);

    /** @} */

private:
    // we allow our dbus stream operators to access this class directly
    friend QDBusArgument &::operator<<(QDBusArgument &argument, const libreGrid::Profile &profile);
    friend const QDBusArgument &::operator>>(const QDBusArgument &argument, libreGrid::Profile &profile);
    
    QScopedPointer<ProfilePrivate> d_ptr;
    Q_DECLARE_PRIVATE(Profile)
};

}

Q_DECLARE_METATYPE(libreGrid::Profile)


#endif // LIBREGRID_PROFILE_H
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
