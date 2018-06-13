- we should find a more practical way to manage multiple hwComm classes for Fan Controllers
- currently there is no hotplug support for sensors as we don't have support for any hotplug enabled devices
- provide lists for all child elements on the dbus
- handle duplicate identifiers across sensor classes

- currently, all configuration changes are done live
    -> a better way would be to set a "dirty" flag on rules and profiles and add a "save" method to write changes to disk
    -> but this would be rahter complicated if the currently edited rule or profile is the active one

