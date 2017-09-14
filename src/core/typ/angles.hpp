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

#pragma once
#include <lib/dev/inc/ptr.hpp>
#include <lib/dev/inc/vecs.hpp>
#include <lib/dev/typ/arr2.hpp>
#include <lib/dev/typ/sz2.hpp>
#include "geometry.hpp"
#include "range.hpp"

namespace core {
//------------------------------------------------------------------------------

dcl_(Angles)
  atr_(tth_t, tth);
  atr_(gma_t, gma);

  Angles();
  Angles(tth_t, gma_t);

  set_(operator=, (rc));
dcl_end

dcl_(AngleMap) SHARED
  dcl_sub_(angle_arr, l::arr2<Angles>) SHARED
    using base::base;
  dcl_end

  dcl_(Key0) COMPARABLE EQ_NE
    atr_(Geometry, geometry);
    atr_(l::sz2, size);
    atr_(ImageCut, cut);
    atr_(l::ij, midPix);

    Key0();
    Key0(Geometry::rc, l::sz2::rc, ImageCut::rc, l::ij::rc midPix);
  dcl_end

  dcl_sub_(Key, Key0) COMPARABLE COMP_OPS
    atr_(tth_t, midTth);

    Key(Geometry::rc, l::sz2::rc, ImageCut::rc, l::ij::rc midPix, tth_t midTth);
    Key(Key0, tth_t midTth);
  dcl_end

  atr_(Key, key);
  atr_(tth_rge, rgeTth);
  atr_(gma_rge, rgeGma);
  atr_(gma_rge, rgeGmaFull);

  AngleMap(Key::rc);

  mth_(Angles::rc, at, (uint i, uint j));
  mth_(Angles::rc, at, (uint i));

  voi_(getGmaIndexes, (gma_rge::rc, uint_vec const*&, uint&, uint&));

private:
  voi_mut_(calculate, ());
  angle_arr::shp angles;

  // sorted
  l::vec<gma_t> gmas;
  uint_vec      gmaIndexes;
dcl_end

//------------------------------------------------------------------------------
}
// eof
