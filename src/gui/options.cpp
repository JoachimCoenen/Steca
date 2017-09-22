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

#include "options.hpp"
#include <lib/dev/inc/defs.inc>

namespace gui {
//------------------------------------------------------------------------------

bool dgram_options::operator==(rc that) const {
  return
      norm              == that.norm &&
      isCombined        == that.isCombined &&
      isFixedIntenScale == that.isFixedIntenScale &&
      gammaRange        == that.gammaRange;
}

bool dgram_options::operator!=(rc that) const {
  return !(*this == that);
}

void dgram_options::set(rc that) {
  mut(norm)              = that.norm;
  mut(isCombined)        = that.isCombined;
  mut(isFixedIntenScale) = that.isFixedIntenScale;
  mut(gammaRange)        = that.gammaRange;
}

//------------------------------------------------------------------------------
}
// eof
