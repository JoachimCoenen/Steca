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

#ifndef CORE_IMAGE_HPP
#define CORE_IMAGE_HPP

#include "../typ/def.h"
#include "../typ/range.h"
#include <app_lib/inc/ptr.h>
#include <app_lib/typ/arr2.h>

namespace core {
//------------------------------------------------------------------------------

dcl_(count_arr2)
  using count_rge = Range;
  atr_(count_rge, rgeCount)

  count_arr2(l::sz2);
  count_arr2(rc);

  val_(l::sz2, size, (), cs.sz)
  val_(bool, isEmpty, (), cs.isEmpty())

  val_(inten_t, at, (uint i),         cs.at(i))
  val_(inten_t, at, (uint i, uint j), cs.at(i,j))

  set_inl_(setAt, (uint i,         inten_t c), cs.setAt(i, c);    )
  set_inl_(setAt, (uint i, uint j, inten_t c), cs.setAt(i, j, c); )

  set_(addAt, (uint, uint, inten_t))

private:
  l::arr2<inten_t> cs;  // could be optimized int8/16/32
dcl_end

dcl_(Image) SHARED
  Image();
  Image(l::sz2::rc);
  Image(count_arr2::rc);

  mth_(l::sz2, size, ())
  set_(clear, ())
  mth_(bool, isEmpty, ())

  mth_(inten_t, inten, (uint i, uint j))
  mth_(inten_t, inten, (uint i))
  set_(setInten, (uint i, uint j, inten_t val))
  set_(addInten, (uint i, uint j, inten_t val))
  set_(addIntens, (rc))

  val_(inten_rge, rgeInten, (), intens->rgeCount)

private:
  QSharedPointer<count_arr2> intens;
dcl_end

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

  atr_(eTransform, val)

  // clamps val appropriately
  ImageTransform(uint val = ROTATE_0);

  // adds/removes the mirror flag
  ImageTransform mirror(bool on) const;

  // rotates only; keeps the mirror flag
  ImageTransform rotateTo(rc) const;

  // rotates by one quarter-turn
  ImageTransform nextRotate() const;

  val_(bool, isTransposed, (), 0 != (val & 1))

  bool operator==(rc that) const {
    return val == that.val;
  }
dcl_end

//------------------------------------------------------------------------------
}
#endif
// eof
