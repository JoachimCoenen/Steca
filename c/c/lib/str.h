// c

#ifndef C_C_STR_H
#define C_C_STR_H

#include <c/c/h>

typedef char*       pstr;
typedef char const* pcstr;

_c_struct (str)
  _var (sz_t, size)
  _ptr(char,   p)

  _c_con (str, (pcstr))
  _c_des(str)
_c_struct_end(str)

_nc_sub_c_struct (str)
  _con (str, (pcstr))
  _con (str, (rc))
  _con (str, (rval))

  _op(pcstr, p)
_nc_sub_c_struct_end

C_CPP(typedef c::str::rc strc;)

//------------------------------------------------------------------------------
#endif
// eof
