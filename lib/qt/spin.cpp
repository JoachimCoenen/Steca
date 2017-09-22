// (qt_lib)

#include "spin.hpp"
#include "inc/defs.inc"
#include "font.hpp"

namespace l_qt {
//------------------------------------------------------------------------------

spin::spin() {}

spin::ref spin::digitWidth(uint n)
  SET_(base::setMaximumWidth(mWidth(*this, n)))

spin::ref spin::min(int val)
  SET_(base::setMinimum(val))

spin::ref spin::max(int val)
  SET_(base::setMaximum(val))

spin::ref spin::val(int val)
  SET_(base::setValue(val))

int spin::val() const
  RET_(base::value())

//------------------------------------------------------------------------------

spinInt::spinInt() {
  connect(this, static_cast<void (Self::*)(int)>(&Self::valueChanged), [this](int) {
    emit valChg(val());
  });
}

spinUint::spinUint() {
  min(0);
  connect(this, static_cast<void (Self::*)(int)>(&Self::valueChanged), [this](int) {
    emit valChg(l::to_uint(val()));
  });
}

spinPint::spinPint() {
  min(1);
  connect(this, static_cast<void (Self::*)(int)>(&Self::valueChanged), [this](int) {
    emit valChg(l::pint(l::to_uint(val())));
  });
}

//------------------------------------------------------------------------------

spinReal::spinReal(uint decimals_) {
  decimals(decimals_);
  connect(this, static_cast<void (Self::*)(double)>(&Self::valueChanged), [this](double) {
    emit valChg(val());
  });
}

spinReal::ref spinReal::decimals(uint n)
  SET_(base::setDecimals(int(n)))

spinReal::ref spinReal::digitWidth(uint n)
  SET_(base::setMaximumWidth(mWidth(*this, n)))

spinReal::ref spinReal::min(real val)
  SET_(base::setMinimum(val))

spinReal::ref spinReal::max(real val)
  SET_(base::setMaximum(val))

spinReal::ref spinReal::val(real val)
  SET_(base::setValue(val))

real spinReal::val() const
  RET_(base::value())

//------------------------------------------------------------------------------
}
// eof
