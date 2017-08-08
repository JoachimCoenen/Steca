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
#include "thehub.hpp"
#include <dev_lib/defs.inc>

namespace gui {
//------------------------------------------------------------------------------

using rw_n = ModelDatasets::rw_n;
using cl_n = ModelDatasets::cl_n;

//------------------------------------------------------------------------------

ModelFiles::ModelFiles(Hub& hub) : HubAccess(hub) {
  setCheckable(true);
}

cl_n ModelFiles::cols() const {
  return cl_n(1);
}

rw_n ModelFiles::rows() const {
  return rw_n(hub.numFiles());
}

l_qt::var ModelFiles::cell(rw_n, cl_n) const {
  return l_qt::var();
}

ModelFiles::ref ModelFiles::check(rw_n, bool) {
  return *this;
}

bool ModelFiles::isChecked(rw_n) const {
  return false;
}

//------------------------------------------------------------------------------

ModelDatasets::ModelDatasets() {
  setCheckable(true);
}

cl_n ModelDatasets::cols() const {
  return cl_n(1);
}

rw_n ModelDatasets::rows() const {
  return rw_n(0);
}

l_qt::var ModelDatasets::cell(rw_n, cl_n) const {
  return l_qt::var("file");
}

//------------------------------------------------------------------------------

ModelMetadata::ModelMetadata() {
  setCheckable(true);
}

cl_n ModelMetadata::cols() const {
  return cl_n(1);
}

rw_n ModelMetadata::rows() const {
  return rw_n(0);
}

l_qt::var ModelMetadata::cell(rw_n, cl_n) const {
  return l_qt::var();
}

//------------------------------------------------------------------------------
}
// eof
