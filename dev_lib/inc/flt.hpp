// (dev_lib)

#pragma once

#include "../defs.hpp"
#include "num.hpp"

namespace l {
//------------------------------------------------------------------------------

template <typename Real>
dcl_(RealNum)
  atr_(Real, val);

  explicit RealNum(Real val_ = flt_nan) : val(val_) {}
  cop_(Real) RET_(val)

#define REALNUM_DCL_OP(op) \
  set_(operator op, (rc that))  SET_(mut(val) op that.val) \
  set_(operator op, (Real val)) SET_(mut(val) op val)

  REALNUM_DCL_OP(=)
  REALNUM_DCL_OP(+=)
  REALNUM_DCL_OP(-=)
  REALNUM_DCL_OP(*=)
  REALNUM_DCL_OP(/=)
#undef REALNUM_DCL_OP

#define REALNUM_DCL_OP(op) \
  mth_(Self, operator op, (rc that)) RET_(RealNum(*this) op that)

  REALNUM_DCL_OP(+)
  REALNUM_DCL_OP(-)
  REALNUM_DCL_OP(*)
  REALNUM_DCL_OP(/)
#undef REALNUM_DCL_OP

  bol_(isnan,   ()) RET_(l::isnan(val))
  bol_(isfin,   ()) RET_(l::isfin(val))
  bol_(isinf,   ()) RET_(l::isinf(val))
  bol_(isnormal,()) RET_(l::isnormal(val))

  mth_(int64, floor, ()) RET_(l::floor(val))
  mth_(int64, ceil,  ()) RET_(l::ceil(val))
  mth_(int64, round, ()) RET_(l::round(val))
  mth_(Self,  abs,   ()) RET_(RealNum(l::abs(val)))

  mth_(rc, notnan, (rc that)) RET_(l::notnan(*this, that))

  mth_(Self, pow, (uint n)) RET_(l::pow(val, n))
dcl_end

using Flt32 = RealNum<flt32>;
using Flt64 = RealNum<flt64>;

using Real = Flt64;

//------------------------------------------------------------------------------
}
// eof
