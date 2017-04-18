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

#include "../typ/def.hpp"
#include "../typ/range.hpp"
#include <c2/c/ptr.h>
#include <c2/cpp/arr2.hpp>

namespace core {
//------------------------------------------------------------------------------

dcl_struct_(count_arr2)
  using count_rge = Range;
  atr_(count_rge, rgeCount)

  count_arr2(c::sz2);
  count_arr2(rc);

  val_(c::sz2, size, (), cs.sz)
  bol_inl_(isEmpty, (), cs.isEmpty())

  val_(inten_t, at, (uint i),         cs.at(i))
  val_(inten_t, at, (uint i, uint j), cs.at(i,j))

  voi_mut_inl_(setAt, (uint i,         inten_t c), { cs.setAt(i, c);    })
  voi_mut_inl_(setAt, (uint i, uint j, inten_t c), { cs.setAt(i, j, c); })

  voi_mut_(addAt, (uint, uint, inten_t))

private:
  c::arr2<inten_t> cs;  // could be optimized int8/16/32
dcl_end

dcl_struct_(Image) SHARED
  Image();
  Image(c::sz2::rc);
  Image(count_arr2::rc);

  mth_(c::sz2, size, ())
  voi_mut_(clear, ())
  bol_(isEmpty, ())

  mth_(inten_t, inten, (uint i, uint j))
  mth_(inten_t, inten, (uint i))
  voi_mut_(setInten, (uint i, uint j, inten_t val))
  voi_mut_(addInten, (uint i, uint j, inten_t val))
  voi_mut_(addIntens, (rc))

  val_(inten_rge, rgeInten, (), intens->rgeCount)

private_
  c::shared<count_arr2> intens;
dcl_end

dcl_struct_(ImageTransform)
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

  bol_inl_(isTransposed, (), 0 != (val & 1))

  bool operator==(rc that) const {
    return val == that.val;
  }
dcl_end

//------------------------------------------------------------------------------
}
#endif
// eof
