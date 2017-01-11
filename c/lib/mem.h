// c

#ifndef C_MEM_H
#define C_MEM_H

#include "../h"

typedef void*            pvoid;
typedef void const*      pcvoid;

_c_c_struct(mem)
  _c_var(size_t, size)
  _c_ptr(void,   p)

  _c_con(mem, (size_t))
  _c_con(mem, (size_t, pcvoid))
  _c_des(mem)
_c_c_struct_end(mem)

_c_cpp_c_struct(mem) NO_COPY(mem)
  _c_cpp_con(mem, (size_t))
  _c_cpp_con(mem, (size_t, pcvoid))
  _c_cpp_con(mem, (rval))

  _c_cpp_op(pcvoid, p)
_c_cpp_c_struct_end(mem)

//------------------------------------------------------------------------------
#endif
// eof
