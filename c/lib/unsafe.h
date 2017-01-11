// c

#ifndef C_UNSAFE_H
#define C_UNSAFE_H

#include "../def/def_h"
#include <utility>

typedef void*            pvoid;
typedef void const*      pcvoid;

#if _c_cpp_

namespace unsafe { namespace mem {

pvoid alloc(size_t);
void  free(pvoid);
pvoid cpy(size_t, pcvoid);

}}

#endif

//------------------------------------------------------------------------------
#endif
// eof
