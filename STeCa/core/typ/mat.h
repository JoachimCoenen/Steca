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

#ifndef CORE_MAT_H
#define CORE_MAT_H

#include <c2/c/c_def.h>
#include <c2/c/lib/num.h>
#include <c2/c/lib/trg.h>

#define NS__ core
#define ST__ vec3

// core_vec3
_c_struct
  _atr (real, _0) _atr (real, _1) _atr (real, _2)
  _c_con (real, real, real)
_c_struct_end

// core::vec3
_cpp_struct C_BASE_CONS COMPARABLE EQ_NE
_cpp_struct_end

#undef  ST__
#define ST__ mat3

// core_mat3
_c_struct
  _atr (real, _00) _atr (real, _01) _atr (real, _02)
  _atr (real, _10) _atr (real, _11) _atr (real, _12)
  _atr (real, _20) _atr (real, _21) _atr (real, _22)
  _c_con (real, real, real,
         real, real, real,
         real, real, real)
_c_struct_end

// core::mat3
_cpp_struct C_BASE_CONS COMPARABLE EQ_NE
  _mth (mat3, transposed, ())
  _mth (mat3, operator*,  (rc))
  _mth (vec3, operator*,  (vec3::rc))

  _fry (mat3, rotationCWx,  (c::rad))
  _fry (mat3, rotationCWz,  (c::rad))
  _fry (mat3, rotationCCWz, (c::rad))
_cpp_struct_end

#undef ST__
#undef NS__

#endif
