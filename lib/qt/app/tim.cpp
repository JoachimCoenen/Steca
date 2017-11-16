// (lib/qt)

#include "tim.hpp"
#include "../defs.inc"
#include <QDateTime>

namespace l_qt {
//------------------------------------------------------------------------------

static msec_t now_msec() {
  return msec_t(QDateTime::currentMSecsSinceEpoch());
}

//------------------------------------------------------------------------------

tim::tim() : Self(now_msec()) {}

tim::tim(msec_t msec_) : msec(msec_) {}

COMPARABLE_IMPL(tim) {
  RET_COMPARE_IF_NE_THAT(msec)
  return 0;
}

EQ_NE_IMPL(tim)

//------------------------------------------------------------------------------
}
// eof
