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

#ifndef CORE_ANGLES_H
#define CORE_ANGLES_H

#include "geometry.hpp"
#include "range.hpp"
#include <c2/c/arr2.h>
#include <c2/c/ptr.h>

namespace core {
//------------------------------------------------------------------------------

dcl_struct (Angles)
  _atr (tth_t, tth)
  _atr (gma_t, gma)

  Angles();
  Angles(tth_t, gma_t);

  _set (operator=, (rc))

dcl_struct_end

typedef Range tth_rge;
typedef Range gma_rge;

dcl_struct (AngleMap) SHARED
  using angle_arr = c::arr2<Angles>;

  dcl_struct (Key) COMPARABLE EQ_NE
    _atr (Geometry, geometry)
    _atr (c::sz2, size)
    _atr (ImageCut, cut)
    _atr (c::ij, midPix)
    _atr (tth_t, midTth)

    Key(Geometry::rc, c::sz2::rc, ImageCut::rc, c::ij::rc midPix, tth_t midTth);
  dcl_struct_end

  _atr (Key, key)
  _atr (tth_rge, rgeTth)
  _atr (gma_rge, rgeGma)
  _atr (gma_rge, rgeGmaFull)

  AngleMap(Key::rc);

  _mth (Angles::rc, at, (uint i, uint j))
  _voi (getGmaIndexes, (gma_rge::rc, uint_vec const*&, uint&, uint&))

_private
  _voi_mut (calculate, ())
  c::shared<angle_arr> angles;

  // sorted
  c::vec<gma_t> gmas;
  uint_vec      gmaIndexes;
};

//------------------------------------------------------------------------------
}
#endif
// eof
