// (qt_lib)

#include "spin.h"
#include <dev_lib/inc/defs_cpp.h>

namespace l_qt {
//------------------------------------------------------------------------------

spin::spin() {
  decimals(0);
}

spin::ref spin::decimals(uint n) SET_(base::setDecimals(int(n)))

spin::ref spin::digitWidth(uint n) SET_(base::setMaximumWidth(int(n * fontMetrics().width('m'))))

spin::ref spin::min(real val) SET_(base::setMinimum(val))

spin::ref spin::max(real val) SET_(base::setMaximum(val))

spin::ref spin::val(real val) SET_(base::setValue(val))

real spin::val() const VAL_(base::value())

//------------------------------------------------------------------------------
}
// eof