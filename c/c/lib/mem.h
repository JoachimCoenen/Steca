// c

#ifndef C_C_MEM_H
#define C_C_MEM_H

#include "../def/def_data"

typedef void*       pvoid;
typedef void const* pcvoid;

#define DATA_NS    c
#define DATA_NAME  mem

_c_data
  _c_var(sz_t, sz)
  _c_ptr(void, p)
  _c_con(sz_t, pcvoid)
_c_data_end

_cpp_struct
  _con(sz_t)
  _con(sz_t, pcvoid)
  _con(rc)
  _con(rval)
  _des

  _op_inline (pcvoid(), p)
_cpp_struct_end

#undef DATA_NS
#undef DATA_NAME

#endif
// eof
