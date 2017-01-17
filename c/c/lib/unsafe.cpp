// c

#include "unsafe.h"
#include <c/c/cpp>
#include <memory>
#include <string.h>

namespace unsafe { namespace mem {
//------------------------------------------------------------------------------

pvoid alloc(sz_t sz) {
  return ::malloc(sz); // TODO abrt if null
}

pvoid realloc(pvoid p, sz_t sz) {
  return ::realloc(p, sz); // TODO abrt if null
}

void free(pvoid p) {
  if (p) ::free(p);
}

pvoid cpy(sz_t size, pcvoid src) {
  if (!src) return nullptr;
  pvoid p = alloc(size);
  ::memcpy(p, src, size);
  return p;
}

TEST("unsafe",
  free(alloc(0));
  free(realloc(nullptr, 9));
)

//------------------------------------------------------------------------------
}}
// eof
