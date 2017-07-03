// (dev_lib)

#include "cache.hpp"
#include "../defs.inc"

namespace l {
//------------------------------------------------------------------------------

cache_base::cache_base(pint maxItems_) : maxItems(maxItems_) {}

cache_base::~cache_base() {}

void cache_base::prepAdd() {
  if (rollOver)
    trim(0);
  else if (size() >= maxItems)
    trim(to_sz(maxItems * .7));
}

cache_base::mru_t cache_base::next() {
  auto mru = nextMru++;
  if (0 == nextMru)
    rollOver = true;  // overflow
  return mru;
}

void cache_base::touch(mru_t& mru) {
  if (mru + 1 == nextMru)
    return;
  mru = next();
}

//------------------------------------------------------------------------------
}
// eof
