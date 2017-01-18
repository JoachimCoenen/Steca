// c

#ifndef C_C_TRG_H
#define C_C_TRG_H

#include "../h_data"
#include "num.h"

#define DATA_NS   c
#define DATA_NAME deg

_c_data
  _c_atr(real, val)
  _c_con((real))
_c_data_end

_ns_dcl(struct rad;)

_cpp_struct
  _con_c_fwd((real val = 0), (val))
  _xon((rad))
  _op_inline(real(), val)

  _mth(rad, toRad, ())
  _mth(deg, operator+, (rc))
  _mth(deg, operator*, (real))
  _mth(deg, normalized, ())

  _mth(real, sin, ())
  _mth(real, cos, ())
_cpp_struct_end

#undef DATA_NAME
#define DATA_NAME rad

_c_data
  _c_atr(real, val)
  _c_con((real))
_c_data_end

_cpp_struct
  _con_c_fwd((real val = 0), (val))
  _op_inline(real(), val)

  _xon((deg))
  _mth(deg, toDeg, ())
  _mth(rad, operator+, (rc))
  _mth(rad, operator*, (real))
  _mth(rad, normalized, ())

  _mth(real, sin, ())
  _mth(real, cos, ())
_cpp_struct_end

#undef DATA_NAME
#undef DATA_NS

#endif
