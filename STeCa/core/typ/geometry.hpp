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

#ifndef CORE_GEOMETRY_H
#define CORE_GEOMETRY_H

#include "def.hpp"
#include <c2/c/arr2.h>

namespace core {
//------------------------------------------------------------------------------

dcl_struct (Geometry) COMPARABLE EQ_NE
  _cst (real, MIN_DETECTOR_DISTANCE)
  _cst (real, MIN_DETECTOR_PIXEL_SIZE)
  _cst (real, DEF_DETECTOR_DISTANCE)
  _cst (real, DEF_DETECTOR_PIXEL_SIZE)

  Geometry();

  _atr (real,  detectorDistance)
  _atr (real,  pixSize)
  _atr (c::ij, midPixOffset)
dcl_struct_end

dcl_struct (ImageCut) COMPARABLE EQ_NE
  _atr (uint, left)
  _atr (uint, top)
  _atr (uint, right)
  _atr (uint, bottom)

  ImageCut();
  ImageCut(uint left, uint top, uint right, uint bottom);

  c::sz2 marginSize() const;
};

//------------------------------------------------------------------------------
}
#endif
// eof
