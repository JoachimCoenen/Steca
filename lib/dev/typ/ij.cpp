// (lib/dev)

#include "ij.hpp"
#include "../inc/defs.inc"

namespace l {
//------------------------------------------------------------------------------

ij::ij() : ij(0, 0) {}
ij::ij(int i_, int j_) : i(i_), j(j_) {}

TEST_("ij",
  ij ij, ij12(1, 2);
  CHECK_EQ(0, ij.i);   CHECK_EQ(0, ij.j);
  CHECK_EQ(1, ij12.i); CHECK_EQ(2, ij12.j);
)

uij::uij() : uij(0, 0) {}
uij::uij(uint i_, uint j_) : i(i_), j(j_) {}

TEST_("uij",
  uij uij, uij12(1, 2);
  CHECK_EQ(0, uij.i);   CHECK_EQ(0, uij.j);
  CHECK_EQ(1, uij12.i); CHECK_EQ(2, uij12.j);
)

COMPARABLE_IMPL(ij) {
  RET_COMPARE_IF_NE_THAT(i)
  RET_COMPARE_IF_NE_THAT(j)
  return 0;
}

EQ_NE_IMPL(ij)

TEST_("ij",
  ij ij0, ij00(0, 0), ij12(1, 2);
  CHECK_EQ(ij0, ij00); CHECK_NE(ij00, ij12); CHECK_NE(ij12, ij0);
)

COMPARABLE_IMPL(uij) {
  RET_COMPARE_IF_NE_THAT(i)
  RET_COMPARE_IF_NE_THAT(j)
  return 0;
}

EQ_NE_IMPL(uij)

TEST_("uij",
  uij uij0, uij00(0, 0), uij12(1, 2);
  CHECK_EQ(uij0, uij00); CHECK_NE(uij00, uij12); CHECK_NE(uij12, uij0);
)

//------------------------------------------------------------------------------
}
// eof
