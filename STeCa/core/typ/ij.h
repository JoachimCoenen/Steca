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

#ifndef CORE_IJ_H
#define CORE_IJ_H

#include <c/c/c_def.h>

#define NS___ core
#define DS___ IJ

_c_struct
  _atr(int, i)
  _atr(int, j)
  _c_con()
  _c_con(int, int)
_c_struct_end

_cpp_struct WITH_C_BASE_CONS COMPARABLE EQ_NE
_cpp_struct_end

#undef DS___
#undef NS___

#endif
// eof
