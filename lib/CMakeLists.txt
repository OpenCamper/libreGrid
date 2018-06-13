set(libreGrid_SRC
    types.cpp
)

add_library(libreGrid ${libreGrid_SRC})
target_link_libraries(libreGrid Qt5::Core Qt5::DBus)
