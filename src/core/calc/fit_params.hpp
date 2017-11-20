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

/** @file
 * Fitting parameters
 */
#pragma once

#include "../typ/geometry.hpp"
#include "../typ/image.hpp"
#include "../data/sets.hpp"
#include "../typ/angles.hpp"
#include "reflection.hpp"
#include "lens.hpp"
#include <lib/dev/typ/cache.hpp>

namespace core {

struct Session;

namespace calc {
//------------------------------------------------------------------------------

/** Contains a full set of parameters for fitting. */
dcl_(FitParams) SHARED CLONED
  using Reflection = calc::Reflection;

  FitParams();

  /// how normnalized
  atr_(core::calc::eNorm, norm) = core::calc::eNorm::NONE;

  /// background polynomial degree
  atr_(uint,   bgPolyDegree);
  /// 2theta ranges of background
  atr_(Ranges, bgRanges);

  /// correction image (if any)
  atr_(Image::shp,      corrImage);

  /// reflections
  atr_(reflection_vec,  refls);
  /// current (active) reflection
  atr_(Reflection::shp, currRefl);
  /// scale of intensity display
  atr_(l::peal,         intenScale);
  /// used geometry
  atr_(Geometry, geometry);

  /// is correction enabled ?
  atr_(bool,            corrEnabled);
  /// map of 2theta/gamma angles
  mth_(AngleMap::shp,   angleMap, (data::Set::rc));
  /// do average intensities in a histogram bucket
  atr_(bool,            avgIntens);
  /// the correction image
  mth_(Image::shp,      intensCorr, ());

  /// make a lens to display images
  mth_(ImageLens::shr,  imageLens,
       (Image::rc, data::CombinedSets::rc, bool trans, bool cut));
  /// make a lens to display datasets
  mth_(DatasetLens::shr, datasetLens,
        (data::CombinedSets::rc, data::CombinedSet::rc, bool trans, bool cut));
  /// make a histogram curve
  mth_(Curve, makeCurve, (calc::DatasetLens::rc, gma_rge::rc));

  /// calculate the background average (used for normalization by background)
  mth_(real, calcAvgBackground, (data::CombinedSets::rc, data::CombinedSet::rc));
  /// calculate the background average (used for normalization by background)
  mth_(real, calcAvgBackground, (data::CombinedSets::rc));

private:
  friend struct core::Session;

  // lazy computation
  mutable Image::shp lazyIntensCorr;
  voi_(calcIntensCorr, ());

  mutable l::cache<AngleMap::Key,AngleMap> angleMapCache;
  mutable bool corrHasNaNs;

  FitParams(rc) = default;
dcl_end

//------------------------------------------------------------------------------
}}
// eof
