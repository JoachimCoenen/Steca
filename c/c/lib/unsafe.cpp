// c

#include "unsafe.h"
#include <c/c/cpp>
#include <memory>
#include <string.h>

namespace unsafe { namespace mem {
//------------------------------------------------------------------------------

pvoid alloc(sz_t size) {
  return ::malloc(size); // TODO abrt if null
}

void free(pvoid p) {
  if (p) ::free(p);
}

pvoid cpy(sz_t size, pcvoid src) {
  pvoid p = alloc(size);
  ::memcpy(p, src, size);
  return p;
}

//------------------------------------------------------------------------------
}}
// eof
