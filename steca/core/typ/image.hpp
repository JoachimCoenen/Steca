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

#include "def.hpp"
#include "range.hpp"
#include <dev_lib/inc/ptr.hpp>
#include <dev_lib/typ/arr2.hpp>

namespace core {
//------------------------------------------------------------------------------

dcl_(ImageTransform)
  enum eTransform {
    ROTATE_0        = 0,  // no transform
    ROTATE_1        = 1,  // one quarter
    ROTATE_2        = 2,  // two quarters
    ROTATE_3        = 3,  // three quarters
    MIRROR          = 4,
    MIRROR_ROTATE_0 = MIRROR | ROTATE_0,
    MIRROR_ROTATE_1 = MIRROR | ROTATE_1,
    MIRROR_ROTATE_2 = MIRROR | ROTATE_2,
    MIRROR_ROTATE_3 = MIRROR | ROTATE_3,
  };

  atr_(eTransform, val);

  // clamps val appropriately
  ImageTransform(uint val = ROTATE_0);

  // adds/removes the mirror flag
  ImageTransform mirror(bool on) const;

  // rotates only; keeps the mirror flag
  ImageTransform rotateTo(rc) const;

  // rotates by one quarter-turn
  ImageTransform nextRotate() const;

  bol_(isTransposed, ()) RET_(0 != (val & 1))

  bol_(operator==, (rc that)) RET_(val == that.val)
dcl_end

dcl_(count_arr2) SHARED
  using count_rge = Range;
  atr_(count_rge, rgeCount);

  count_arr2(l::sz2, inten_t = inten_t(0));
  count_arr2(rc);

  mth_(l::sz2, size, ()) RET_(cs.sz)
  bol_(isEmpty, ()) RET_(cs.isEmpty())

  mth_(inten_t, at, (uint i))         RET_(cs.at(i))
  mth_(inten_t, at, (uint i, uint j)) RET_(cs.at(i,j))

  set_(setAt, (uint i,         inten_t c)) SET_(cs.setAt(i, c))
  set_(setAt, (uint i, uint j, inten_t c)) SET_(cs.setAt(i, j, c))

  set_(addAt, (uint, uint, inten_t));

private:
  l::arr2<inten_t> cs;  // could be optimized int8/16/32
dcl_end

dcl_(Image) SHARED
  Image();
  Image(l::sz2::rc, inten_t = inten_t(0));
  Image(count_arr2::rc);

  mth_(l::sz2, size, ());
  set_(clear, ());
  bol_(isEmpty, ());

  mth_(inten_t, inten, (uint i, uint j));
  mth_(inten_t, inten, (uint i));
  set_(setInten, (uint i, uint j, inten_t val));
  set_(addInten, (uint i, uint j, inten_t val));
  set_(addIntens, (rc));

  mth_(inten_rge, rgeInten, ()) RET_(intens->rgeCount)

private:
  count_arr2::sh intens;
dcl_end

//------------------------------------------------------------------------------
}
// eof
