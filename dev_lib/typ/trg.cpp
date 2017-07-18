// (dev_lib)

#include "trg.hpp"
#include "../defs.inc"
#include <cmath>

namespace l {
//------------------------------------------------------------------------------

deg::deg(real val_) : val(val_) {}
deg::deg(rad::rc r) : deg(r.toDeg()) {}

TEST_("deg",
  CHECK_EQ(real(deg(rad(M_PI_2))), 90);
)

deg::ref deg::operator=(rc that) SET_(mut(val) = that.val)

rad deg::toRad() const {
  return rad(val * (M_PI / 180));
}

deg deg::operator+(rc that) const {
  return deg(val + that.val);
}

deg deg::operator-(rc that) const {
  return deg(val - that.val);
}

deg deg::operator-() const {
  return deg(-val);
}

deg deg::operator*(real fac) const {
  return deg(val * fac);
}

deg deg::operator/(real fac) const {
  return deg(val / fac);
}

deg deg::normalized() const {
  static real const over = 360;
  real norm = fmod(val, over);
  if (norm < 0) norm += over;
  return deg(norm);
}

real deg::sin() const {
  return ::sin(toRad());
}

real deg::cos() const {
  return ::cos(toRad());
}

//------------------------------------------------------------------------------

rad::rad(real val_) : val(val_) {}
rad::rad(deg::rc d) : rad(d.toRad()) {}

TEST_("rad",
  CHECK_EQ(real(rad(deg(90))), M_PI_2);
)

rad::ref rad::operator=(rc that) SET_(mut(val) = that.val)

deg rad::toDeg() const {
  return deg(val * (180 / M_PI));
}

rad rad::operator+(rc that) const {
  return rad(val + that.val);
}

rad rad::operator-(rc that) const {
  return rad(val - that.val);
}

rad rad::operator-() const {
  return rad(-val);
}

rad rad::operator*(real fac) const {
  return rad(val * fac);
}

rad rad::operator/(real fac) const {
  return rad(val / fac);
}

rad rad::normalized() const {
  static real const over = 2 * M_PI;
  real norm = fmod(val, over);
  if (norm < 0) norm += over;
  return rad(norm);
}

real rad::sin() const {
  return ::sin(*this);
}

real rad::cos() const {
  return ::cos(*this);
}

//------------------------------------------------------------------------------
}
// eof
