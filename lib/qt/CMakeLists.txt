cmake_minimum_required (VERSION 3.1)
include (../../cmake.options)

set (lib lib_qt)

find_package (Qt5 5.2.1 REQUIRED COMPONENTS Core Gui Widgets)
set (CMAKE_AUTOMOC ON)

file (GLOB_RECURSE files *)
add_library (${lib} ${files})

target_link_libraries (${lib}
  Qt5::Core Qt5::Gui Qt5::Widgets
)

# eof
