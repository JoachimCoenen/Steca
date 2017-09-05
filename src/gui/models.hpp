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
#include "refhub.hpp"
#include <lib/qt/model.hpp>
#include <core/data/files.hpp>

namespace gui {
//------------------------------------------------------------------------------

dcl_sub_(ModelFiles, l_qt::lst_model)
  ModelFiles(Hub const&);

  mth_(cl_n, cols, ());
  mth_(rw_n, rows, ());

  mth_(str,       head, (cl_n));
  mth_(l_qt::var, cell, (rw_n, cl_n));

  set_(check,     (rw_n, bool));
  bol_(isChecked, (rw_n));

private:
  core::data::Files::sh files;
dcl_end

dcl_sub2_(ModelDatasets, RefHub, l_qt::lst_model)
  ModelDatasets(Hub&);

  mth_(cl_n, cols, ());
  mth_(rw_n, rows, ());

  mth_(str,       head, (cl_n));
  mth_(l_qt::var, cell, (rw_n, cl_n));

  voi_(fixColumns,  (l_qt::lst_view&));
  bol_(rightAlign,  (cl_n));

  set_(check, (rw_n, bool));
  bol_(isChecked, (rw_n));

private:
  uint_vec metaCols;  // shown metadata
  bol_(grouped, ());
  mth_(uint, numLeadCols, ());
dcl_end

dcl_sub2_(ModelMetadata, RefHub, l_qt::lst_model)
  ModelMetadata(Hub&);

  enum { clTAG, clVAL };

  mth_(cl_n, cols, ());
  mth_(rw_n, rows, ());

  mth_(str,       head, (cl_n));
  mth_(l_qt::var, cell, (rw_n, cl_n));

  set_(check, (rw_n, bool));
  bol_(isChecked, (rw_n));

private:
  core::data::CombinedSet::sh dataset;
dcl_end

//------------------------------------------------------------------------------
}
// eof
