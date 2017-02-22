// (c)

#ifndef C_C_TRG_H
#define C_C_TRG_H

#include "../c_def.h"
#include "num.h"

#define NS___ c
#define DS___ deg

_c_struct
  _atr(real, val)
  _c_con(real)
_c_struct_end

_ns_dcl(struct rad;)

_cpp_struct
  _con_c_fwd((real val = 0), (val))
  _xon(rad)
  _op_inline(real(), val)

  _mth(rad, toRad, ())
  _mth(deg, operator+, (rc))
  _mth(deg, operator*, (real))
  _mth(deg, normalized, ())

  _mth(real, sin, ())
  _mth(real, cos, ())
_cpp_struct_end

#undef DS___
#define DS___ rad

_c_struct
  _atr(real, val)
  _c_con(real)
_c_struct_end

_cpp_struct
  _con_c_fwd((real val = 0), (val))
  _op_inline(real(), val)

  _xon(deg)
  _mth(deg, toDeg, ())
  _mth(rad, operator+, (rc))
  _mth(rad, operator*, (real))
  _mth(rad, normalized, ())

  _mth(real, sin, ())
  _mth(real, cos, ())
_cpp_struct_end

#undef DS___
#undef NS___

#endif
