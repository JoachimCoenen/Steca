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

#ifndef CORE_SESSION_HPP
#define CORE_SESSION_HPP

#include "data/data.hpp"
#include "typ/angles.hpp"
#include <c2/c/num.h>
#include <c2/cpp/cache.hpp>
#include <c2/cpp/strlst.hpp>

namespace core {
//------------------------------------------------------------------------------

dcl_struct_(Session)
  enum class eNorm {
    NONE,
    MONITOR, DELTA_MONITOR, DELTA_TIME, BACKGROUND,
  };

  atr_(c::str_lst, normStrLst)

  Session();

  mth_(AngleMap::sh,   angleMap, (data::Set::rc))
  atr_(AngleMap::Key0, angleMapKey0) // current
  atr_(ImageCut,       imageCut)
  atr_(c::sz2,         imageSize)

  atr_(bool,    avgScaleIntens)
  atr_(c::peal, intenScale)

private_
  mutable c::cache<AngleMap::Key,AngleMap> angleMapCache;
dcl_end

//------------------------------------------------------------------------------
}
#endif
// eof
