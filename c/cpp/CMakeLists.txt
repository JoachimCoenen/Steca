set (lib c_cpp)

file (GLOB_RECURSE files *)
add_library (${lib} ${files})

#eof
