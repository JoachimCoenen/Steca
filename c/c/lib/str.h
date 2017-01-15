// c

#ifndef C_C_STR_H
#define C_C_STR_H

#include <c/c/def/def_data>

typedef char*       pstr;
typedef char const* pcstr;

#define DATA_NS    c
#define DATA_NAME  str

_c_data
  _var (sz_t, sz)
  _ptr (char, p)

  _c_con (sz_t, pcstr)
_c_data_end

_cpp_struct
  _con (pcstr)
  _con (rc)
  _con (rval)
  _des

  _op_inline (pcstr(), p)
_cpp_struct_end

_cpp_code (typedef c::str::rc strc;)

#undef DATA_NS
#undef DATA_NAME

#endif
// eof
