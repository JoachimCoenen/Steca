cmake_minimum_required (VERSION 3.1)
include (../../cmake.inc)

get_filename_component (lib ${CMAKE_CURRENT_SOURCE_DIR} NAME)

find_package(Qt5 5.5.1 REQUIRED COMPONENTS Core Gui Widgets)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)

file (GLOB_RECURSE files *)
add_library (${lib} ${files} rc.qrc)

target_link_libraries (${lib}
  Qt5::Core Qt5::Gui Qt5::Widgets
)

# eof
