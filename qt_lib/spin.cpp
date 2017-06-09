// (app_lib)

#include "spin.h"
#include "../inc/defs_cpp.h"
#include "../typ_qt/wgt_help.h"

namespace l_qt {
//------------------------------------------------------------------------------

spin::spin() {
  decimals(0);
}

spin::ref spin::decimals(uint n) {
  base::setDecimals(int(n));
  return *this;
}

spin::ref spin::digitWidth(uint n) {
  base::setMaximumWidth(int(n * mWidth(*this)));
  return *this;
}

spin::ref spin::min(real val) {
  base::setMinimum(val);
  return *this;
}

spin::ref spin::max(real val) {
  base::setMaximum(val);
  return *this;
}

spin::ref spin::val(real val) {
  base::setValue(val);
  return *this;
}

real spin::val() const {
  return base::value();
}

//------------------------------------------------------------------------------
}
// eof
