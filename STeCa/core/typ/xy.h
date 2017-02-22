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

#ifndef CORE_XY_H
#define CORE_XY_H

#include <c/c/c_def.h>
#include <c/c/lib/num.h>

#define NS___ core
#define DS___ XY

_c_struct
  _atr(real, x)
  _atr(real, y)
  _c_con()
  _c_con(real, real)
_c_struct_end

_cpp_struct C_BASE_CONS COMPARABLE EQ_NE
  _mth(bool, isDef, ())
_cpp_struct_end

#undef DS___
#undef NS___

#endif
// eof
