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

#pragma once
#include "../typ/geometry.hpp"
#include "../typ/image.hpp"
#include "../data/sets.hpp"
#include "../typ/angles.hpp"
#include "reflection.hpp"
#include "lens.hpp"
#include <lib/dev/typ/cache.hpp>

namespace core { namespace calc {
//------------------------------------------------------------------------------

dcl_(FitParams) SHARED CLONED
  using Reflection = calc::Reflection;

  enum eWhat { NONE, BACKGROUND, PEAK };

  FitParams();

  atr_(core::eNorm, norm)       = core::eNorm::NONE;
  atr_(uint,   bgPolyDegree);
  atr_(Ranges, bgRanges);
  atr_(Image::shp,      corrImage);
  atr_(Ranges, bg);
  atr_(reflection_vec,  refls);
  atr_(Reflection::shp, currRefl);
  atr_(l::peal,         intenScale);
  atr_(Geometry, geometry);

  atr_(bool,            corrEnabled);
  mth_(AngleMap::shp,   angleMap, (data::Set::rc));
  atr_(bool,            avgScaleIntens);

  mth_(Image::shp,      intensCorr, ());

  mth_(ImageLens::shr,  imageLens,
       (Image::rc, data::CombinedSets::rc, bool trans, bool cut));
  mth_(DatasetLens::shr, datasetLens,
        (data::CombinedSets::rc, data::CombinedSet::rc, bool trans, bool cut));
  mth_(Curve, makeCurve, (calc::DatasetLens::rc, gma_rge::rc));

  mth_(real, calcAvgBackground, (data::CombinedSets::rc, data::CombinedSet::rc));
  mth_(real, calcAvgBackground, (data::CombinedSets::rc));

//private: TODO make private
  mutable l::cache<AngleMap::Key,AngleMap> angleMapCache;
  mutable Image::shp intensCorrImage;
  voi_(calcIntensCorr, ());
  mutable bool corrHasNaNs;

private:
  FitParams(rc) = default;
dcl_end

//------------------------------------------------------------------------------
}}
