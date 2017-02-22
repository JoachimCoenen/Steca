// (c)

#ifndef C_C_MEM_H
#define C_C_MEM_H

#include "../c_def.h"

typedef void*       pvoid;
typedef void const* pcvoid;

#define NS___ c
#define DS___ mem

_c_struct
  _atr(sz_t, sz)
  _ptr(void, p)

  _c_con(sz_t, pcvoid)
_c_struct_end

_cpp_struct
  _con(sz_t)
  _con(sz_t, pcvoid)
  _con(rc)
  _con(rval)
  _des()

  _op_inline (pcvoid(), p)
_cpp_struct_end

#undef DS___
#undef NS___

_cpp_code(
namespace c { namespace unsafe {

pvoid alloc(sz_t);
pvoid calloc(sz_t, sz_t);
pvoid realloc(pvoid, sz_t);
void  free(pvoid);
pvoid memcpy(sz_t, pcvoid);

}})

#endif
// eof
