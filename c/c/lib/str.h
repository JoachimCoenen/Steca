// c

#ifndef C_C_STR_H
#define C_C_STR_H

#include "../h_data"

typedef char*       pstr;
typedef char const* pcstr;

#define DATA_NS    c
#define DATA_NAME  str

_c_data
  _c_var(sz_t, sz)
  _c_ptr(char, p)
  _c_con(sz_t, pcstr)
_c_data_end

_cpp_struct COMPARABLE COMP_OPS
  _con(pcstr)
  _con(sz_t maxSz, pcstr)
  _con(rc)
  _con(rval)
  _des

  _op_inline (pcstr(), p)

  _mth(str, trim, ())
  _fry(str, format, (pcstr, ...))

  _cst(str, nul)
_cpp_struct_end

_cpp_code (typedef c::str::rc strc;)

#undef DATA_NAME
#undef DATA_NS

#endif
// eof
