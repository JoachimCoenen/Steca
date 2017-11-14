// (qt_lib)

#include "spin.hpp"
#include "../defs.inc"
#include "font.hpp"

namespace l_qt {
//------------------------------------------------------------------------------

spin::spin(uint digitWidth_) {
  setAlignment(Qt::AlignRight); digitWidth(digitWidth_);
  min(l::val_min(val()));
  max(l::val_max(val()));
}

spin::ref spin::digitWidth(uint n) {
  auto&& w = width_0(*this, n + 3.33); // adjustment for the spinner
  base::setMinimumWidth(w);
  base::setMaximumWidth(w);
  RTHIS;
}

spin::ref spin::min(int val)
  SET_(base::setMinimum(val))

spin::ref spin::max(int val)
  SET_(base::setMaximum(val))

spin::ref spin::val(int val)
  SET_(base::setValue(val))

int spin::val() const
  RET_(base::value())

//------------------------------------------------------------------------------

spinInt::spinInt(uint digitWidth) : base(digitWidth) {
  connect(this, static_cast<void (Self::*)(int)>(&Self::valueChanged), [this](int) {
    emit valChg(val());
  });
}

spinUint::spinUint(uint digitWidth) : base(digitWidth) {
  min(0);
  connect(this, static_cast<void (Self::*)(int)>(&Self::valueChanged), [this](int) {
    emit valChg(l::to_uint(val()));
  });
}

spinPint::spinPint(uint digitWidth) : base(digitWidth) {
  min(1);
  connect(this, static_cast<void (Self::*)(int)>(&Self::valueChanged), [this](int) {
    emit valChg(l::pint(l::to_uint(val())));
  });
}

//------------------------------------------------------------------------------

spinReal::spinReal(uint digitWidth_, uint decimals_) {
  decimals(decimals_); min(-l::real_inf); max(+l::real_inf);
  setAlignment(Qt::AlignRight); digitWidth(digitWidth_);
  connect(this, static_cast<void (Self::*)(double)>(&Self::valueChanged), [this](double) {
    emit valChg(val());
  });
}

spinReal::ref spinReal::decimals(uint n)
  SET_(base::setDecimals(int(n)))

spinReal::ref spinReal::digitWidth(uint n) {
  auto&& w = width_0(*this, n + 3.33); // adjustment for the spinner
  base::setMinimumWidth(w);
  base::setMaximumWidth(w);
  RTHIS;
}

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
// eof DOCS
