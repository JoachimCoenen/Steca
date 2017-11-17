/*******************************************************************************
 * Steca2 - StressTextureCalculator ver. 2
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

/**@file
 * Common types
*/

#pragma once

#include <lib/dev/inc/flt.hpp>
#include <lib/dev/typ/gon.hpp>
#include <lib/dev/typ/vec.hpp>
#include <lib/dev/typ/xy.hpp>
#include "core/typ/range.hpp"

namespace core {
//------------------------------------------------------------------------------

// angles
use_deg_(tth_t)   ///< 2theta (beam / detector)
use_deg_(gma_t)   ///< gamma (around the diffraction cone)

use_deg_(omg_t)   ///< omega (sample)
use_deg_(phi_t)   ///< phi (sample)
use_deg_(chi_t)   ///< chi (sample)

struct Ranges;

using tth_rge = Range;      ///< range of tth
using gma_rge = Range;      ///< range of gma

using peak_t  = l::xy;      ///< a peak position+height
using fwhm_t  = l::Flt32;   ///< a peak width

using inten_t   = l::Flt32; ///< intensity (neutron count)
using inten_vec = l::vec<inten_t>;
using inten_rge = Range;

struct Geometry;

//------------------------------------------------------------------------------
}

namespace core { namespace calc {
//------------------------------------------------------------------------------

struct FitParams;

//------------------------------------------------------------------------------
}}
// eof
