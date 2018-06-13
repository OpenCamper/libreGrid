include_directories(${LIBUSB_1_INCLUDE_DIRS} ${LM_SENSORS_INCLUDE_DIRS} ../contrib/spdlog/include)

# enable trace and debug logging
add_definitions(-DSPDLOG_TRACE_ON -DSPDLOG_DEBUG_ON)

set(libreGridd_SRC
    Settings.cpp

    Controller.cpp
    Device.cpp
    Fan.cpp
    Sensor.cpp
    Temperature.cpp
    Profile.cpp
    simpleRule.cpp
    libreGridd.cpp
    
    RuleFactory.cpp
    
    hwComm/FanController.cpp
    hwComm/USB.cpp
    hwComm/Fan.cpp
    hwComm/GRIDv3.cpp
    hwComm/GRIDv3Fan.cpp
    
    hwComm/Sensor.cpp
    hwComm/Temperature.cpp
    hwComm/lmSensors.cpp
    hwComm/lmSensorsChip.cpp
    hwComm/lmSensorsTemperature.cpp
)

if(NVML_LIBRARY)
    set(libreGridd_SRC ${libreGridd_SRC}
        hwComm/NVidia.cpp
        hwComm/NVidiaSensor.cpp
        hwComm/NVidiaTemperature.cpp
    )
    add_definitions(-DHAS_NVML)
endif(NVML_LIBRARY)

qt5_add_dbus_adaptor(
    libreGridd_SRC 
    ../dbus/libre.Grid1.controller.xml
    Controller.h
    libreGrid::server::Controller
)

qt5_add_dbus_adaptor(
    libreGridd_SRC
    ../dbus/libre.Grid1.device.xml
    Device.h
    libreGrid::server::Device
)

qt5_add_dbus_adaptor(
    libreGridd_SRC
    ../dbus/libre.Grid1.fan.xml
    Fan.h
    libreGrid::server::Fan
)

qt5_add_dbus_adaptor(
    libreGridd_SRC
    ../dbus/libre.Grid1.sensor.xml
    Sensor.h
    libreGrid::server::Sensor
)

qt5_add_dbus_adaptor(
    libreGridd_SRC
    ../dbus/libre.Grid1.temperature.xml
    Temperature.h
    libreGrid::server::Temperature
)

qt5_add_dbus_adaptor(
    libreGridd_SRC
    ../dbus/libre.Grid1.profile.xml
    Profile.h
    libreGrid::server::Profile
)

qt5_add_dbus_adaptor(
    libreGridd_SRC
    ../dbus/libre.Grid1.profile.simpleRule.xml
    simpleRule.h
    libreGrid::server::simpleRule
)



add_executable(libreGridd ${libreGridd_SRC})
target_link_libraries(libreGridd
    libreGrid
    
    Qt5::Core
    Qt5::DBus
    
    ${LIBUSB_1_LIBRARIES}
    ${LM_SENSORS_LIBRARIES}
    ${NVML_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
)

install(FILES ../dbus/libre.Grid1.controller.conf DESTINATION /etc/dbus-1/system.d)
