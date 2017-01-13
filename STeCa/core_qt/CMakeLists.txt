get_filename_component (lib ${CMAKE_CURRENT_SOURCE_DIR} NAME)

find_package (Qt5Core    REQUIRED)

set (CMAKE_AUTOMOC ON)

file (GLOB_RECURSE files *)
add_library (${lib} ${files})

target_link_libraries (${lib}
  Qt5::Core
)

# eof
