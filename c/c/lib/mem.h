// (c)

#ifndef C_C_MEM_H
#define C_C_MEM_H

#include "../c_def.h"

typedef void*       pvoid;
typedef void const* pcvoid;

#define NS__ c
#define ST__ mem

// c_mem
_c_struct
  _atr(sz_t, sz)
  _ptr(void, p)

  _c_con(sz_t, pcvoid)
_c_struct_end

// c::mem
_cpp_struct
  _con(sz_t)
  _con(sz_t, pcvoid)
  _con(rc)
  _con(rval)
  _des()

  _op_inline (pcvoid(), p)
_cpp_struct_end

#undef ST__
#undef NS__

_cpp_code(
namespace c { namespace unsafe {

// these return nullptr if sz_t > 0 and fail
pvoid maybe_alloc(sz_t);
pvoid maybe_calloc(sz_t, sz_t);
pvoid maybe_realloc(pvoid, sz_t);
pvoid maybe_memcpy(sz_t, pcvoid);

// these abort if fail
pvoid alloc(sz_t);
pvoid calloc(sz_t, sz_t);
pvoid realloc(pvoid, sz_t);
pvoid memcpy(sz_t, pcvoid);

void  free(pvoid);
}})

#endif
// eof
