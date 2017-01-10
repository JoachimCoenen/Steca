set (lib c53c)
file (GLOB_RECURSE files *)
add_library (${lib} ${files})

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")

#eof
