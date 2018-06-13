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

#include "Settings.h"
#include <QSettings>
#include <QJsonDocument>
#include <QJsonObject>
#include <QIODevice>
#include <stdexcept>

#include <QDebug>

using namespace libreGrid::server;

Settings *Settings::_instance = nullptr;
QMutex   *Settings::_m = new QMutex();

void settings_to_json(QVariant &target, QStringList keys, const QVariant &val)
{
    // get the map of the target element
    QVariantMap map = target.toMap();
    if (keys.count() == 1)
        map.insert(keys[0], val);
    else {
        // create or get the child map
        QVariant item = map.value(keys[0]);
        QString key = keys[0];

        // remove the key from the QStringList
        keys.pop_front();

        // recurse
        settings_to_json(item, keys, val);

        // write back the updated map
        map.insert(key, item);
    }

    // update our QVariant
    target = map;
}

bool writeJsonFile(QIODevice &device, const QSettings::SettingsMap &map)
{
    // create an empty variant map for our json doc
    QVariant jmap;

    for (auto it = map.begin(); it != map.end(); ++it) {
        // split the key
        QStringList keys = it.key().split('/');

        // update our map
        settings_to_json(jmap, keys, it.value());
    }
   
    QJsonDocument jsonDocument = QJsonDocument::fromVariant(jmap);
    
    if (device.write(jsonDocument.toJson()) == -1)
        return false;

    return true;
}

void json_to_settings(QSettings::SettingsMap &map, const QVariant &json, const QStringList &keys = QStringList())
{
    // if this is not a map, we restore the value
    if (json.type() != QVariant::Map)
        map.insert(keys.join('/'), json);
    else {
        QVariantMap jmap = json.toMap();
        // iterate over our map
        for (auto it = jmap.begin(); it != jmap.end(); ++it) {
            // create a new stringlist
            QStringList newKeys = keys;
            newKeys << it.key();

            // recurse
            json_to_settings(map, it.value(), newKeys);
        }
    }
}

bool readJsonFile(QIODevice &device, QSettings::SettingsMap &map)
{
    QJsonParseError error;
    QByteArray data = device.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data , &error);

    if (error.error != QJsonParseError::NoError)
        return false;

    json_to_settings(map, doc.toVariant());
    
    return true;
}

void Settings::init(const QString filename)
{
    QMutexLocker locker(_m);
    if (_instance == nullptr)
        _instance = new Settings(filename);
}

QSettings *Settings::get(QObject *parent)
{
    {
        QMutexLocker locker(_m);
        if (_instance == nullptr)
            throw(std::runtime_error("Settings::get called before init"));
    }
    return new QSettings(_instance->m_path, _instance->m_format, parent);
}

Settings::Settings(const QString &path)
    : m_path(path)
{
    m_format = QSettings::registerFormat("json", &readJsonFile, &writeJsonFile);
}





// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
