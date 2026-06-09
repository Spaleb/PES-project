cmake_minimum_required(VERSION 3.16)

project(PES_Display VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# We zoeken direct naar Qt6. Geen omwegen meer.
find_package(Qt5 REQUIRED COMPONENTS Widgets Network)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

# Maak de executable
add_executable(PES_Display
    ${PROJECT_SOURCES}
)

# Koppel de bibliotheken (Widgets voor UI, Network voor de verbinding met je script)
target_link_libraries(PES_Display PRIVATE
    Qt5::Widgets
    Qt5::Network
)

include(GNUInstallDirs)
install(TARGETS PES_Display
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
