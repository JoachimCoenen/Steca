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

#include "models.hpp"
#include <c2/h/c_cpp>
#include <c2/cpp/vec.hpp>
#include <c2/gui_qt/var.hpp>

namespace core_qt {
//------------------------------------------------------------------------------

using rw_n = ModelDatasets::rw_n;
using cl_n = ModelDatasets::cl_n;

//------------------------------------------------------------------------------

ModelFiles::ModelFiles() {
  checkable(true);
}

cl_n ModelFiles::cols() const {
  return 1;
}

rw_n ModelFiles::rows() const {
  return 0;
}

c_gui::var* ModelFiles::cell(rw_n, cl_n) const {
  return nullptr;
}

void ModelFiles::check(rw_n, bool) {
}

bool ModelFiles::isChecked(rw_n) const {
  return false;
}

//------------------------------------------------------------------------------

ModelDatasets::ModelDatasets() {
  checkable(true);
}

cl_n ModelDatasets::cols() const {
  return 1;
}

rw_n ModelDatasets::rows() const {
  return 0;
}

c_gui::var* ModelDatasets::cell(rw_n, cl_n) const {
  return new c_qt::var("file");
}

//------------------------------------------------------------------------------

ModelMetadata::ModelMetadata() {
  checkable(true);
}

cl_n ModelMetadata::cols() const {
  return 1;
}

rw_n ModelMetadata::rows() const {
  return 0;
}

c_gui::var* ModelMetadata::cell(rw_n, cl_n) const {
  return nullptr;
}

//------------------------------------------------------------------------------
}
// eof
