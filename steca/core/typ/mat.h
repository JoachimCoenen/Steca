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

#include <dev_lib/inc/defs_h.h>
#include <dev_lib/typ/trg.h>

namespace core {

dcl_(vec3) COMPARABLE EQ_NE
  atr_(real, _0);
  atr_(real, _1);
  atr_(real, _2);

  vec3(real, real, real);
dcl_end

dcl_(mat3) COMPARABLE EQ_NE
  atr_(real, _00); atr_(real, _01); atr_(real, _02);
  atr_(real, _10); atr_(real, _11); atr_(real, _12);
  atr_(real, _20); atr_(real, _21); atr_(real, _22);

  mat3(real, real, real,
       real, real, real,
       real, real, real);

  mth_(mat3, transposed, ());
  mth_(mat3, operator*,  (rc));
  mth_(vec3, operator*,  (vec3::rc));

  fry_(mat3, rotationCWx,  (l::rad));
  fry_(mat3, rotationCWz,  (l::rad));
  fry_(mat3, rotationCCWz, (l::rad));
dcl_end

}

#endif
// eof
