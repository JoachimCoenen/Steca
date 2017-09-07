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
#include <lib/dev/typ/bag.hpp>
#include <core/data/files.hpp>

namespace gui {
//------------------------------------------------------------------------------

dcl_sub2_(Model, RefHub, l_qt::lst_model)
  Model(Hub&);

protected:
  core::data::Files::sh files;
  virtual void newFiles() {}
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(ModelFiles, Model)
  ModelFiles(Hub&);

  mth_(cl_n, cols, ());
  mth_(rw_n, rows, ());

  mth_(str,       head, (cl_n));
  mth_(l_qt::var, cell, (rw_n, cl_n));

  set_(check,     (rw_n, bool));
  bol_(isChecked, (rw_n));

  mth_(core::data::File::sh, at, (rw_n));
dcl_end

dcl_sub_(ModelDatasets, Model)
  ModelDatasets(Hub&);

  mth_(cl_n, cols, ());
  mth_(rw_n, rows, ());

  mth_(str,       head, (cl_n));
  mth_(l_qt::var, cell, (rw_n, cl_n));

  voi_(fixColumns,  (l_qt::lst_view&));
  bol_(rightAlign,  (cl_n));

  set_(check, (rw_n, bool));
  bol_(isChecked, (rw_n));

  atr_(l::pint, groupedBy);
  voi_mut_(groupBy, (l::pint));

private:
  core::data::CombinedSets::sh combinedSets;
  uint_vec metaCols;  // shown metadata

  void newFiles();
  mth_(uint, numLeadCols, ());
  voi_mut_(combineSets, ());
dcl_end

dcl_sub_(ModelMetadata, Model)
  ModelMetadata(Hub&);

  enum { clTAG, clVAL };

  mth_(cl_n, cols, ());
  mth_(rw_n, rows, ());

  mth_(str,       head, (cl_n));
  mth_(l_qt::var, cell, (rw_n, cl_n));

  set_(check, (rw_n, bool));
  bol_(isChecked, (rw_n));

private:
  void newFiles();
  core::data::FilesMetaDict::sh dict;
  core::data::CombinedSet::sh dataset;
  l::bag<str> checked;
dcl_end

//------------------------------------------------------------------------------
}
// eof
