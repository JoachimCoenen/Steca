cmake_minimum_required (VERSION 3.1)
include (../../cmake.inc)

get_filename_component (lib ${CMAKE_CURRENT_SOURCE_DIR} NAME)

set (CMAKE_CXX_STANDARD 11)
if (CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-Wall -Wextra -Wpedantic)
endif ()

find_package (Qt5 5.2.1 REQUIRED COMPONENTS Core Widgets)
set (CMAKE_AUTOMOC ON)

file (GLOB_RECURSE files *)
add_library (${lib} ${files})

target_link_libraries (${lib}
  Qt5::Core Qt5::Widgets
)

# eof
