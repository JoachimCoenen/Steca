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
#include <core/typ/options.hpp>
#include <core/typ/range.hpp>

namespace gui {
//------------------------------------------------------------------------------

dcl_(dgram_options) EQ_NE
  atr_(core::eNorm, norm)       = core::eNorm::NONE; // TODO out to fit_options?
  atr_(bool, isCombined)   = false; // TODO to plot ?
  atr_(bool, isFixedIntenScale) = false; // TODO to plot ?
  atr_(core::Range, gammaRange); // TODO to plot ?
  mut_(set, (rc));
dcl_end

dcl_(image_options) EQ_NE
  atr_(bool, isFixedIntenScale) = false;
  mut_(set, (rc));
dcl_end

//------------------------------------------------------------------------------
}
// eof
