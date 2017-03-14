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
#include <c2/c/c_cpp>
namespace core_qt {
//------------------------------------------------------------------------------

static uint const COLS = 1;

ModelFiles::ModelFiles() {
  checkable(true);
}

ModelFiles::cl_n ModelFiles::cols() const {
  return COLS;
}

ModelFiles::rw_n ModelFiles::rows() const {
  return 3;
}

c_gui::var* ModelFiles::cell(rw_n, cl_n) const {
  return new c_qt::var("file");
}

void ModelFiles::check(rw_n, bool) {
  notify();
}

bool ModelFiles::isChecked(rw_n) const {
  return false;
}

//------------------------------------------------------------------------------
}
// eof
