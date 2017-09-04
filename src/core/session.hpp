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
  Session();

  voi_mut_(clear, ());
  voi_mut_(load, (io::Json::rc)) may_err;
  mth_(io::Json, save, ());

  atr_(data::Files::sh,files);

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

  bol_mut_(addFiles,   (l_io::path_vec::rc)) may_err;
  bol_mut_(remFilesAt, (uint_vec::rc));

  set_(activateFileAt, (uint, bool));
  bol_(isActiveFileAt, (uint));

  set_(activateDatasetAt, (uint, bool));
  bol_(isActiveDatasetAt, (uint));

  set_(setCorrFile,   (l_io::path::rc)) may_err;
  set_(remCorrFile,   ());
  set_(tryEnableCorr, (bool on));


  atr_(data::CombinedSets, collectedDatasets);
  atr_(str_vec,            collectedDatasetsTags);
  atr_(uint_vec,           collectedFromFiles);
  atr_(l::pint,            groupedBy);

  set_(collectDatasetsFromFiles, (uint_vec::rc, l::pint by));

  set_(setImageSize,  (l::sz2)) may_err;

  mth_(calc::ImageLens::sh, imageLens,
        (Image::rc, data::CombinedSets::rc, bool trans, bool cut));
  mth_(calc::DatasetLens::sh, datasetLens,
        (data::CombinedSets::rc, data::CombinedSet::rc,
         eNorm, bool trans, bool cut));
  mth_(Curve, makeCurve, (calc::DatasetLens::rc, gma_rge::rc));

  atr_(uint,   bgPolyDegree);
  atr_(Ranges, bgRanges);

  mth_(real, calcAvgBackground, (data::CombinedSets::rc, data::CombinedSet::rc));
  mth_(real, calcAvgBackground, (data::CombinedSets::rc));

private:
  mutable l::cache<AngleMap::Key,AngleMap> angleMapCache;
  mutable Image::sh intensCorrImage;
  mutable bool corrHasNaNs;

  voi_mut_(updateImageSize, ());
  voi_(calcIntensCorr, ());
dcl_end

//------------------------------------------------------------------------------
}
// eof
