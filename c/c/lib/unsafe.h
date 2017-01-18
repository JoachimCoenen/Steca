// c

#ifndef C_C_UNSAFE_H
#define C_C_UNSAFE_H

#include "../h"

typedef void*       pvoid;
typedef void const* pcvoid;

#if _is_cpp_

namespace unsafe {

pvoid alloc(sz_t);
pvoid calloc(sz_t, sz_t);
pvoid realloc(pvoid, sz_t);
void  free(pvoid);
pvoid memcpy(sz_t, pcvoid);

}

#endif

//------------------------------------------------------------------------------
#endif
// eof
