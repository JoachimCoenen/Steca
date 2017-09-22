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
#include "typ/options.hpp"
#include "data/files.hpp"
#include "io/json.hpp"
#include "typ/curve.hpp"
#include "calc/fit_params.hpp"
#include "typ/geometry.hpp"
#include <lib/dev/inc/num.hpp>
#include <lib/dev/io/path.hpp>
#include <lib/dev/typ/cache.hpp>

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
  atr_(Geometry::sh,   geometry);
  atr_(calc::FitParams::sh,   fp);

  atr_(data::File::shp, corrFile);

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

private:
  mut_(updateImageSize, ());
dcl_end

//------------------------------------------------------------------------------
}
// eof
