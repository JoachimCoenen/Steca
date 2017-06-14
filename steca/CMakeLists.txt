cmake_minimum_required (VERSION 3.1)
include (../cmake.inc)

get_filename_component (app ${CMAKE_CURRENT_SOURCE_DIR} NAME)

find_package (Qt5 5.6.2 REQUIRED COMPONENTS Core Gui Widgets)

include_directories (${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory (core)
add_subdirectory (gui)

add_executable (${app} main.cpp)
add_dependencies (${app} gui core)

target_link_libraries (${app}
  gui core qt_lib dev_lib
  Qt5::Core Qt5::Gui Qt5::Widgets
)

# eof
