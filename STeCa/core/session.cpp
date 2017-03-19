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

#include "session.hpp"
#include <c2/h/c_cpp>

namespace core {
//------------------------------------------------------------------------------

Session::Session()
: normStrLst({"none", "monitor", "Δ monitor", "Δ time", "background"})
, angleMapCache(360)
{}

AngleMap::sh Session::angleMap(data::Set::rc set) const {
  AngleMap::Key key(angleMapKey0, set.midTth());
  auto map = angleMapCache.value(key);
  if (!map)
    map = angleMapCache.add(key, AngleMap::sh(new AngleMap(key)));
  return map;
}


//------------------------------------------------------------------------------
}
// eof
