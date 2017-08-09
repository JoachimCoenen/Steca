// (qt_lib)

#include "spin.hpp"
#include "font.hpp"
#include <dev_lib/defs.inc>

namespace l_qt {
//------------------------------------------------------------------------------

spin::spin() {
  decimals(0);
}

spin::ref spin::decimals(uint n) SET_(base::setDecimals(int(n)))

spin::ref spin::digitWidth(uint n) SET_(base::setMaximumWidth(mWidth(this, n)))

spin::ref spin::min(real val) SET_(base::setMinimum(val))

spin::ref spin::max(real val) SET_(base::setMaximum(val))

spin::ref spin::val(real val) SET_(base::setValue(val))

real spin::val() const RET_(base::value())

//------------------------------------------------------------------------------
}
// eof
