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

#ifndef CORE_GEOMETRY_HPP
#define CORE_GEOMETRY_HPP

#include "def.hpp"
#include <dev_lib/typ/ij.hpp>
#include <dev_lib/typ/sz2.hpp>

namespace core {
//------------------------------------------------------------------------------

dcl_(Geometry) COMPARABLE EQ_NE
  cst_(real, MIN_DETECTOR_DISTANCE);
  cst_(real, MIN_DETECTOR_PIXEL_SIZE);
  cst_(real, DEF_DETECTOR_DISTANCE);
  cst_(real, DEF_DETECTOR_PIXEL_SIZE);

  Geometry();

  atr_(real,  detectorDistance);
  atr_(real,  pixSize);
  atr_(l::ij, midPixOffset);
dcl_end

dcl_(ImageCut) COMPARABLE EQ_NE
  atr_(uint, left);
  atr_(uint, top);
  atr_(uint, right);
  atr_(uint, bottom);

  ImageCut();
  ImageCut(uint left, uint top, uint right, uint bottom);

  l::sz2 marginSize() const;
};

//------------------------------------------------------------------------------
}
#endif
// eof
