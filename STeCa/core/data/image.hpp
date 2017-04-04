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

dcl_struct(count_arr2)
  using count_rge = Range;
  _atr (count_rge, rgeCount)

  count_arr2(c::sz2);
  count_arr2(rc);

  _val (c::sz2, size, (), cs.sz)
  _bol_inl (isEmpty, (), cs.isEmpty())

  _val (inten_t, at, (uint i),         cs.at(i))
  _val (inten_t, at, (uint i, uint j), cs.at(i,j))

  _voi_mut_inl (setAt, (uint i,         inten_t c), { cs.setAt(i, c);    })
  _voi_mut_inl (setAt, (uint i, uint j, inten_t c), { cs.setAt(i, j, c); })

  _voi_mut (addAt, (uint, uint, inten_t))

private:
  c::arr2<inten_t> cs;  // could be optimized int8/16/32
dcl_struct_end

dcl_struct (Image) SHARED
  Image();
  Image(c::sz2::rc);
  Image(count_arr2::rc);

  _mth (c::sz2, size, ())
  _voi_mut (clear, ())
  _bol (isEmpty, ())

  _mth (inten_t, inten, (uint i, uint j))
  _mth (inten_t, inten, (uint i))
  _voi_mut (setInten, (uint i, uint j, inten_t val))
  _voi_mut (addInten, (uint i, uint j, inten_t val))
  _voi_mut (addIntens, (rc))

_private
  c::shared<count_arr2> intens;
dcl_struct_end

dcl_struct (ImageTransform)
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

  _atr (eTransform, val)

  // clamps val appropriately
  ImageTransform(uint val = ROTATE_0);

  // adds/removes the mirror flag
  ImageTransform mirror(bool on) const;

  // rotates only; keeps the mirror flag
  ImageTransform rotateTo(rc) const;

  // rotates by one quarter-turn
  ImageTransform nextRotate() const;

  _bol_inl (isTransposed, (), 0 != (val & 1))

  bool operator==(rc that) const {
    return val == that.val;
  }
dcl_struct_end

//------------------------------------------------------------------------------
}
#endif
// eof
