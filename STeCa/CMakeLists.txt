get_filename_component (app ${CMAKE_CURRENT_SOURCE_DIR} NAME)

find_package (Qt5Core    REQUIRED)
find_package (Qt5Gui     REQUIRED)
find_package (Qt5Widgets REQUIRED)

include_directories (${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory (core)
add_subdirectory (core_qt)  # Qt(Core)-dependent parts
add_subdirectory (gui)

add_executable (${app} main.cpp)

target_link_libraries (${app}
  gui core_qt core c_qt c_cpp c_c
  Qt5::Core Qt5::Gui Qt5::Widgets
)

if (UNIX)
  target_link_libraries (${app} pthread)
endif ()

# eof
