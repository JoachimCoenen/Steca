cmake_minimum_required (VERSION 3.1)
include (../cmake.inc)

get_filename_component (app ${CMAKE_CURRENT_SOURCE_DIR} NAME)

find_package (Qt5 5.5.1 REQUIRED COMPONENTS Core Gui Widgets)
set (CMAKE_AUTOMOC ON)

add_definitions (-DWITH_TESTS)
if (WITH_COVERAGE)
  add_compile_options(--coverage)
endif ()

include_directories (${CMAKE_HOME_DIRECTORY}/steca)

set (srcdirs steca qt_lib dev_lib)
foreach (dir ${srcdirs})
  set (d ${CMAKE_HOME_DIRECTORY}/${dir})
  file (GLOB_RECURSE fs ${d}/*.cpp)
  set (files ${files} ${fs})
endforeach (dir)

list(REMOVE_ITEM files ${CMAKE_HOME_DIRECTORY}/steca/main.cpp)

add_executable (${app} ${files} ${files})

target_link_libraries (${app}
  Qt5::Core Qt5::Gui Qt5::Widgets
)

if (UNIX)
  target_link_libraries (${app} pthread)
endif ()

if (WITH_COVERAGE)
  target_link_libraries (${app} gcov)
endif ()

# eof
