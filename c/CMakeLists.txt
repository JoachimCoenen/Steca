set (lib c2c)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")

file (GLOB_RECURSE files *)
add_library (${lib} ${files})

#eof
