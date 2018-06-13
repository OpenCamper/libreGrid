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

#ifndef LIBREGRID_SERVER_PROFILE_H
#define LIBREGRID_SERVER_PROFILE_H

#include <QObject>
#include <QUuid>
#include <QDBusConnection>

#include "Settings.h"

namespace libreGrid
{
namespace server
{

/**
 * @todo write docs
 */
class Profile : public QObject
{
    Q_OBJECT

    Q_CLASSINFO("D-Bus Interface", "libre.Grid1.profile")
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QStringList rules READ rules)
    Q_PROPERTY(bool valid READ valid)

public:
    /**
     * Destructor
     */
    ~Profile();

    /**
     * Constructor
     *
     * @param parent TODO
     */
    Profile(QDBusConnection &dbus, QObject *parent, QUuid uuid = QUuid());

    const QUuid &identifier() const;

    QString name() const;
    QStringList rules() const;


    void setName(const QString &name);

    bool valid() const;

    void deleteProfile();

public slots:
    bool addRule(const QString &rule);
    void remRule(const QString &rule);

signals:
    void nameChanged(const QString &name);
    void ruleRemoved(const QString &rule);

private:
    // our settings object
    QSettings *m_settings;
    QUuid m_identifier;
    QString m_name;

    QDBusConnection m_dbus;
    QString m_dbusPath;

};

}
}

#endif // LIBREGRID_SERVER_PROFILE_H
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
