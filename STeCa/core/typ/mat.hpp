/*******************************************************************************
 * STeCa2 - StressTextureCalculator ver. 2
 *
 * Copyright (C) 2016-7 Forschungszentrum Jülich GmbH
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the COPYING and AUTHORS files for more details.
 ******************************************************************************/

#ifndef CORE_MAT_HPP
#define CORE_MAT_HPP

#include <c2/h/c_def.h>
#include <c2/c/num.h>
#include <c2/c/trg.h>

namespace core {

dcl_struct (vec3) COMPARABLE EQ_NE
  _atr (real, _0) _atr (real, _1) _atr (real, _2)
  vec3(real, real, real);
dcl_struct_end

dcl_struct (mat3) COMPARABLE EQ_NE
  _atr (real, _00) _atr (real, _01) _atr (real, _02)
  _atr (real, _10) _atr (real, _11) _atr (real, _12)
  _atr (real, _20) _atr (real, _21) _atr (real, _22)
  mat3(real, real, real,
       real, real, real,
       real, real, real);

  _mth (mat3, transposed, ())
  _mth (mat3, operator*,  (rc))
  _mth (vec3, operator*,  (vec3::rc))

  _fry (mat3, rotationCWx,  (c::rad))
  _fry (mat3, rotationCWz,  (c::rad))
  _fry (mat3, rotationCCWz, (c::rad))
dcl_struct_end

}

#endif
// eof
