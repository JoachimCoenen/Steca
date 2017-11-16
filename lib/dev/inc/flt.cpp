// (lib/dev)

#include "flt.hpp"
#include "defs.inc"

namespace l {
//------------------------------------------------------------------------------

TEST_("Flt32",
  CHECK(Flt32().isnan());
  CHECK(!Flt32().isfin());
  CHECK(!Flt32().isinf());
  CHECK(!Flt32().isnormal());

  CHECK_EQ(1, Flt32(-1).abs());
)

//------------------------------------------------------------------------------
}
// eof
