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

#include "calc/lens.hpp"
#include "data/data.hpp"
#include "typ/angles.hpp"
#include "typ/curve.hpp"

#include <dev_lib/inc/num.hpp>
#include <dev_lib/typ/cache.hpp>

namespace core {
//------------------------------------------------------------------------------

dcl_(Session)
  atr_(str_vec, normStrLst);

  Session();

  mth_(AngleMap::sh,   angleMap, (data::Set::rc));
  atr_(AngleMap::Key0, angleMapKey0); // current

  atr_(ImageTransform, imageTransform);
  atr_(ImageCut,       imageCut);
  atr_(l::sz2,         imageSize);

  atr_(bool,           avgScaleIntens);
  atr_(l::peal,        intenScale);

  atr_(bool,           corrEnabled);
  mth_(Image::sh,      intensCorr, ());

  atr_(data::File::sh, corrFile);
  atr_(Image::sh,      corrImage);

  mth_(calc::ImageLens::sh, imageLens,
        (Image::rc, data::CombinedSets::rc, bool trans, bool cut));
  mth_(calc::DatasetLens::sh, datasetLens,
        (data::CombinedSet::rc, data::CombinedSets::rc,
         eNorm, bool trans, bool cut));
  mth_(Curve, makeCurve, (calc::DatasetLens::rc, gma_rge::rc));

  atr_(uint,   bgPolyDegree);
  atr_(Ranges, bgRanges);

  mth_(real, calcAvgBackground, (data::CombinedSet::rc));
  mth_(real, calcAvgBackground, (data::CombinedSets::rc));

private:
  mutable l::cache<AngleMap::Key,AngleMap> angleMapCache;
  mutable Image::sh intensCorrImage;
  mutable bool corrHasNaNs;

  void calcIntensCorr() const;
dcl_end

//------------------------------------------------------------------------------
}
// eof
