set (lib c_cpp)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/..)

file (GLOB_RECURSE files *)
add_library (${lib} ${files})

#eof
