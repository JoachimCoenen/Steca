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

#include <dev_lib/defs.hpp>
#include <dev_lib/inc/flt.hpp>
#include <dev_lib/typ/trg.hpp>
#include <dev_lib/typ/vec.hpp>
#include <dev_lib/typ/ij.hpp>
#include <dev_lib/typ/xy.hpp>

namespace core {
//------------------------------------------------------------------------------

using tth_t = l::deg;
using omg_t = l::deg;
using phi_t = l::deg;
using chi_t = l::deg;

using gma_t = l::deg;

struct Range;

using tth_rge = Range;
using gma_rge = Range;

using peak_t = l::xy;
using fwhm_t = l::Flt32;

using inten_t   = l::Flt32;
using inten_vec = l::vec<inten_t>;
using inten_rge = Range;

//------------------------------------------------------------------------------
}
// eof
