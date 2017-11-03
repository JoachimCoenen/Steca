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
#include "reflection_info.hpp"

namespace core { namespace calc {
//------------------------------------------------------------------------------

dcl_(itf_t)
  atr_(inten_t, inten);
  atr_(tth_t,   tth);
  atr_(fwhm_t,  fwhm);

  itf_t();
  itf_t(inten_t, tth_t, fwhm_t);

  set_(operator=,  (rc));
  set_(operator+=, (rc));
dcl_end

using itfs_t = l::vec<itf_t>;

// Interpolates reflection infos to a single point using idw.
itf_t interpolateValues(l::deg searchRadius, ReflectionInfos::rc infos,
                        l::deg alpha, l::deg beta);

ReflectionInfos interpolate(ReflectionInfos::rc,
    l::deg alphaStep, l::deg betaStep, l::deg idwRadius,
    l::deg averagingAlphaMax, l::deg averagingRadius, real inclusionTreshold);

//------------------------------------------------------------------------------
}}
// eof
