// c

#include "unsafe.h"
#include "../cpp"
#include <memory>
#include <string.h>

namespace unsafe {
//------------------------------------------------------------------------------

pvoid alloc(sz_t sz) {
  return (sz > 0) ? ::malloc(sz) : nullptr; // TODO abrt if null
}

pvoid realloc(pvoid p, sz_t sz) {
  return ::realloc(p, sz); // TODO abrt if null
}

void free(pvoid p) {
  if (p) ::free(p);
}

pvoid memcpy(sz_t size, pcvoid src) {
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
}
// eof
