cmake_minimum_required(VERSION 3.1)
project(libreGrid)

# use a modern standard
set (CMAKE_CXX_STANDARD 14)

# custom cmake files
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Create code from a list of Qt designer ui files.
set(CMAKE_AUTOUIC ON)

# find the QtCore and QtDBus libraries
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5DBus CONFIG REQUIRED)

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)

# Find the USB library
find_package(libusb-1.0 REQUIRED)

# Find the lm_sensors library
find_package(lm_sensors REQUIRED)

# idk why we need this, as Qt5Core is linked against pthread, but without we get DSO errors
find_package (Threads)

# DEBUG
#add_definitions(-g)

# check if we have the NVML installed
find_library(NVML_LIBRARY NAMES nvml nvidia-ml)
if(NVML_LIBRARY)
    message("Using NVidida Management Library ${NVML_LIBRARY}")
endif(NVML_LIBRARY)

# be pedantic
add_definitions(-pedantic -Wall)

include_directories(include)

add_subdirectory(lib)
add_subdirectory(daemon)

enable_testing()
add_subdirectory(tests)
