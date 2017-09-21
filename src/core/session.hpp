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
#include "data/files.hpp"
#include "data/fit.hpp"
#include "io/json.hpp"
#include "typ/angles.hpp"
#include "typ/curve.hpp"
#include <lib/dev/inc/num.hpp>
#include <lib/dev/typ/cache.hpp>
#include <lib/dev/io/path.hpp>

namespace core {
//------------------------------------------------------------------------------

extern str_vec const normStrLst;

dcl_(Session)
  using Range  = core::Range;
  using Ranges = core::Ranges;

  Session();

  mut_(clear, ());
  mth_mut_(data::Files::sh, load, (io::Json::rc)) may_err;
  mth_(io::Json, save, ());

  atr_(data::Files::sh, files);
  atr_(data::Fit::sh,   fit);

  atr_(Geometry, geometry);

  mth_(AngleMap::shp,   angleMap, (data::Set::rc));

  atr_(ImageTransform,  imageTransform);
  atr_(ImageCut,        imageCut);
  atr_(l::sz2,          imageSize);

  atr_(bool,            avgScaleIntens);
  atr_(l::peal,         intenScale);

  atr_(bool,            corrEnabled);
  mth_(Image::shp,      intensCorr, ());

  atr_(data::File::shp, corrFile);
  atr_(Image::shp,      corrImage);

  mth_mut_(data::Files::sh, addFiles,   (l_io::path_vec::rc)) may_err;
  mth_mut_(data::Files::sh, remFilesAt, (uint_vec::rc));

  mth_mut_(data::Files::sh, activateFileAt, (uint, bool));

  set_(setCorrFile,   (l_io::path::rc)) may_err;
  set_(remCorrFile,   ());
  set_(tryEnableCorr, (bool on));

  mut_(setBg, (Ranges::rc));
  mut_(addBg, (Range::rc));
  mut_(remBg, (Range::rc));

  mut_(setRefl, (Range::rc));

  set_(setImageSize,  (l::sz2)) may_err;

  mth_(calc::ImageLens::shp, imageLens,
        (Image::rc, data::CombinedSets::rc, bool trans, bool cut));
  mth_(calc::DatasetLens::shp, datasetLens,
        (data::CombinedSets::rc, data::CombinedSet::rc,
         eNorm, bool trans, bool cut));
  mth_(Curve, makeCurve, (calc::DatasetLens::rc, gma_rge::rc));

  atr_(uint,   bgPolyDegree);
  atr_(Ranges, bgRanges);

  mth_(real, calcAvgBackground, (data::CombinedSets::rc, data::CombinedSet::rc));
  mth_(real, calcAvgBackground, (data::CombinedSets::rc));

  dcl_(dgram_options) EQ_NE
    atr_(core::eNorm, norm)       = core::eNorm::NONE; // TODO out to fit_options?
    atr_(bool, isDgramCombined)   = false; // TODO to plot ?
    atr_(bool, isFixedIntenScale) = false; // TODO to plot ?
    atr_(core::Range, gammaRange); // TODO to plot ?
    mut_(set, (rc));
  dcl_end

  dcl_(image_options) EQ_NE
    atr_(bool, isFixedIntenScale) = false;
    mut_(set, (rc));
  dcl_end

  atr_(dgram_options, dgramOptions);
  atr_(image_options, imageOptions);

  voi_(makeDgram, (Curve& dgram, Curve& bgFitted, Curve& bg, curve_vec& refls,
                   data::CombinedSets::rc, data::CombinedSet const*,
                   data::Fit::rc, dgram_options::rc));
private:
  mutable l::cache<AngleMap::Key,AngleMap> angleMapCache;
  mutable Image::shp intensCorrImage;
  mutable bool corrHasNaNs;

  mut_(updateImageSize, ());
  voi_(calcIntensCorr, ());
dcl_end

//------------------------------------------------------------------------------
}
// eof
