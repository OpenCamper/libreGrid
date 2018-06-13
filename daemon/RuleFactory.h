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

#ifndef LIBREGRID_SERVER_RULEFACTORY_H
#define LIBREGRID_SERVER_RULEFACTORY_H

#include <QObject>
#include <QMutex>
#include "Settings.h"
#include "simpleRule.h"

namespace libreGrid
{
namespace server
{

/**
 * the rule factory creates and manages all Rule instances
 */
class RuleFactory : public QObject
{
public:

    /**
     * singleton getter
     */
    static RuleFactory* instance();

    /**
     * Destructor
     */
    ~RuleFactory();
    
    /**
     * return the rule object for the given identifier
     * 
     * @returns nullptr if the rule does not exists
     */
    simpleRule *getRule(QString identifier);
    simpleRule *getRule(QUuid   identifier);
    
    /**
     * create a new rule
     * 
     * @returns the new rule
     */
    simpleRule *createRule();

private:
    /**
     * Hidden constructor
     */
    RuleFactory();
    
    static RuleFactory  *_instance;
    static QMutex       *_instance_m;
    
    // our settings object
    QSettings           *m_settings;
    
    // our list of currently known rules
    QMap<QString,simpleRule*>   m_rules;
    QMutex                      *m_rules_mutex;

};

}
}

#endif // LIBREGRID_SERVER_RULEFACTORY_H
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
