// c

#ifndef C_C_UNSAFE_H
#define C_C_UNSAFE_H

#include <c/c/h>

typedef void*       pvoid;
typedef void const* pcvoid;

#if _is_cpp_

namespace unsafe { namespace mem {

pvoid alloc(sz_t);
void  free(pvoid);
pvoid cpy(sz_t, pcvoid);

}}

#endif

//------------------------------------------------------------------------------
#endif
// eof
