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

#include <c/c/h>

namespace core {
//------------------------------------------------------------------------------
// 2D point, integers

_struct (IJ)

  _var (int, i)
  _var (int, j)

  IJ();           // (0,0)
  IJ(int, int);

  COMPARABLE EQ_NE

_struct_end

//------------------------------------------------------------------------------
}
#endif
