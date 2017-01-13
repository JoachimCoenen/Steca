// c

#ifndef C_C_MEM_H
#define C_C_MEM_H

#include <c/c/h>

typedef void*            pvoid;
typedef void const*      pcvoid;

_c_struct (mem)
  _var (sz_t, size)
  _ptr(void,   p)

  _c_con (mem, (sz_t))
  _c_con (mem, (sz_t, pcvoid))
  _c_des(mem)
_c_struct_end(mem)

_nc_sub_c_struct (mem) NO_COPY(mem)
  _con (mem, (sz_t))
  _con (mem, (sz_t, pcvoid))
  _con (mem, (rval))

  _op(pcvoid, p)
_nc_sub_c_struct_end

//------------------------------------------------------------------------------
#endif
// eof
