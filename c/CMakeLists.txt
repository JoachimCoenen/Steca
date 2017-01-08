set (lib c53c)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")

file (GLOB_RECURSE files "*")
add_library (${lib} ${files})

target_link_libraries (${lib}
  Qt5::Core
  Qt5::Gui
  Qt5::Widgets
)

#eof
