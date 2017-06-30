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

#ifndef CORE_MODELS_HPP
#define CORE_MODELS_HPP

#include <qt_lib/model.hpp>

namespace core_qt {
//------------------------------------------------------------------------------

dcl_sub_(ModelFiles, l_qt::lst_model)
  ModelFiles();
  mth_(cl_n, cols, ());
  mth_(rw_n, rows, ());
  mth_(l_qt::var, cell, (rw_n, cl_n));

  set_(check, (rw_n, bool));
  mth_(bool, isChecked, (rw_n));
dcl_end

dcl_sub_(ModelDatasets, l_qt::lst_model)
  ModelDatasets();
  mth_(cl_n, cols, ());
  mth_(rw_n, rows, ());
  mth_(l_qt::var, cell, (rw_n, cl_n));
dcl_end

dcl_sub_(ModelMetadata, l_qt::lst_model)
  ModelMetadata();
  mth_(cl_n, cols, ());
  mth_(rw_n, rows, ());
  mth_(l_qt::var, cell, (rw_n, cl_n));
dcl_end

//------------------------------------------------------------------------------
}
#endif
// eof
