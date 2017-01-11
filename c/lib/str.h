// c

#ifndef C_STR_H
#define C_STR_H

#include "../def/def_num"

typedef char*       pstr;
typedef char const* pcstr;

_c_c_struct(str)
  _c_var(size_t, size)
  _c_ptr(char,   p)

  _c_con(str, (pcstr))
  _c_des(str)
_c_c_struct_end(str)

_c_cpp_c_struct(str) NO_COPY(str)
  _c_cpp_con(str, (pcstr))
  _c_cpp_con(str, (rval))

  _c_cpp_op(pcstr, p)
_c_cpp_c_struct_end(str)

//------------------------------------------------------------------------------
#endif
// eof
