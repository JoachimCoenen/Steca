// (dev_lib)

#pragma once
#include "../defs.hpp"
#include "../inc/num.hpp"

namespace l {
//------------------------------------------------------------------------------

struct rad;

dcl_(deg)
  atr_(real, val);

  explicit deg(real = l::flt_nan);
  explicit deg(rad const&);

  cop_(real) RET_(val)

  set_(operator=, (rc));

  mth_(rad, toRad,      ());
  mth_(deg, operator+,  (rc));
  mth_(deg, operator-,  (rc));
  mth_(deg, operator-,  ());
  mth_(deg, operator*,  (real));
  mth_(deg, operator/,  (real));
  mth_(deg, normalized, ());

  mth_(real, sin, ());
  mth_(real, cos, ());
dcl_end

dcl_(rad)
  atr_(real, val);

  explicit rad(real = l::flt_nan);
  explicit rad(deg const&);

  cop_(real) RET_(val)

  set_(operator=, (rc));

  mth_(deg, toDeg,      ());
  mth_(rad, operator+,  (rc));
  mth_(rad, operator-,  (rc));
  mth_(rad, operator-,  ());
  mth_(rad, operator*,  (real));
  mth_(rad, operator/,  (real));
  mth_(rad, normalized, ());

  mth_(real, sin, ());
  mth_(real, cos, ());
dcl_end

//------------------------------------------------------------------------------
}
// eof
