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

#include "typ/angles.hpp"
#include "data/data.hpp"
#include <c2/cpp/cache.hpp>

namespace core {
//------------------------------------------------------------------------------

dcl_struct (Session)
  enum class eNorm {
    NONE,
    MONITOR, DELTA_MONITOR, DELTA_TIME, BACKGROUND,
  };

  _atr (c::vec<c::str>, normStrLst)

  Session();

  _mth (AngleMap::sh, angleMap, (data::Set::rc))

  _atr (AngleMap::Key0, angleMapKey0) // current

_private
  mutable c::cache<AngleMap::Key,AngleMap> angleMapCache;
dcl_struct_end

//------------------------------------------------------------------------------
}
#endif
// eof
