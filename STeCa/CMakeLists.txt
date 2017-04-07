get_filename_component (app ${CMAKE_CURRENT_SOURCE_DIR} NAME)

find_package (Qt5Core    REQUIRED)
find_package (Qt5Gui     REQUIRED)
find_package (Qt5Widgets REQUIRED)
set (CMAKE_AUTOMOC ON)

include_directories (${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory (core)     # computing and data core
add_subdirectory (core_qt)  # Qt-dependent core parts
add_subdirectory (gui)      # Qt-gui

add_executable (${app} main.cpp)
add_dependencies (${app} gui core_qt core c2_gui_qt c2_qt c2_gui c2_cpp c2_c)

target_link_libraries (${app}
  gui core_qt core c2_gui_qt c2_qt c2_gui c2_cpp c2_c
  Qt5::Core Qt5::Gui Qt5::Widgets
)

if (UNIX)
  target_link_libraries (${app} pthread)
endif ()

# eof
