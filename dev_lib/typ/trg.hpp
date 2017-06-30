// (dev_lib)

#pragma once

#include "../inc/defs.hpp"

namespace l {
//------------------------------------------------------------------------------

struct rad;

dcl_(deg)
  atr_(real, val);

  explicit deg(real = 0);
  explicit deg(rad const&);

  cop_(real) VAL_(val)

  set_(operator=, (rc));

  mth_(rad, toRad,      ());
  mth_(deg, operator+,  (rc));
  mth_(deg, operator*,  (real));
  mth_(deg, normalized, ());

  mth_(real, sin, ());
  mth_(real, cos, ());
dcl_end

dcl_(rad)
  atr_(real, val);

  explicit rad(real = 0);
  explicit rad(deg const&);

  cop_(real) VAL_(val)

  set_(operator=, (rc));

  mth_(deg, toDeg,      ());
  mth_(rad, operator+,  (rc));
  mth_(rad, operator*,  (real));
  mth_(rad, normalized, ());

  mth_(real, sin, ());
  mth_(real, cos, ());
dcl_end

//------------------------------------------------------------------------------
}
// eof
