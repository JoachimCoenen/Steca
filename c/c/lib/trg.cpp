// c

#include "trg.h"
#include "../cpp"
#include <cmath>

c_deg::c_deg(real val_) : val(val_) {}
c_rad::c_rad(real val_) : val(val_) {}

namespace c {
//------------------------------------------------------------------------------

TEST("deg",
  CHECK_EQ(real(deg(rad(M_PI_2))), 90);
)

deg::deg(rad r) : deg(r.toDeg()) {}

rad deg::toRad() const {
  return val * (M_PI / 180);
}

deg deg::operator+(rc that) const {
  return val + that.val;
}

deg deg::operator*(real fac) const {
  return val * fac;
}

deg deg::normalized() const {
  static real const over = 360;
  real norm = fmod(val, over);
  if (norm < 0) norm += over;
  return norm;
}

real deg::sin() const {
  return ::sin(toRad());
}

real deg::cos() const {
  return ::cos(toRad());
}

//------------------------------------------------------------------------------

TEST("rad",
  CHECK_EQ(real(rad(deg(90))), M_PI_2);
)

rad::rad(deg d) : rad(d.toRad()) {}

deg rad::toDeg() const {
  return val * (180 / M_PI);
}

rad rad::operator+(rc that) const {
  return val + that.val;
}

rad rad::operator*(real fac) const {
  return val * fac;
}

rad rad::normalized() const {
  static real const over = 2 * M_PI;
  real norm = fmod(val, over);
  if (norm < 0) norm += over;
  return norm;
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
