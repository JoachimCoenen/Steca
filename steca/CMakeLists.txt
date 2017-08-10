cmake_minimum_required (VERSION 3.1)
include (../cmake.options)

get_filename_component (app ${CMAKE_CURRENT_SOURCE_DIR} NAME)

find_package (Qt5 5.2.1 REQUIRED COMPONENTS Core Gui Widgets Multimedia Network)

include_directories (${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory (core_legacy)
add_subdirectory (core)
add_subdirectory (gui)

add_executable (${app} main.cpp manifest.cpp)
add_dependencies (${app} core_legacy core gui)

target_link_libraries (${app}
  gui core core_legacy qt_lib dev_lib
  Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Multimedia Qt5::Network
)

# eof
