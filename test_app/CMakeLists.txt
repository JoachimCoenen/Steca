get_filename_component (app ${CMAKE_CURRENT_SOURCE_DIR} NAME)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../c)

file (GLOB_RECURSE files *.h *.cpp)
add_executable (${app} ${files})

target_link_libraries (${app}
  Qt5::Core
  Qt5::Gui
  Qt5::Widgets
  c53c
)

# eof
