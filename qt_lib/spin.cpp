// (qt_lib)

#include "spin.h"
#include <dev_lib/inc/defs_cpp.h>

namespace l_qt {
//------------------------------------------------------------------------------

spin::spin() {
  decimals(0);
}

spin::ref spin::decimals(uint n) {
  base::setDecimals(int(n)); RT
}

spin::ref spin::digitWidth(uint n) {
  base::setMaximumWidth(int(n * fontMetrics().width('m'))); RT
}

spin::ref spin::min(real val) {
  base::setMinimum(val); RT
}

spin::ref spin::max(real val) {
  base::setMaximum(val); RT
}

spin::ref spin::val(real val) {
  base::setValue(val); RT
}

real spin::val() const {
  return base::value();
}

//------------------------------------------------------------------------------
}
// eof
