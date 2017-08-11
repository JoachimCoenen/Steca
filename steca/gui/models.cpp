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
#include <qt_lib/inc/defs.inc>

namespace gui {
//------------------------------------------------------------------------------

using rw_n = ModelDatasets::rw_n;
using cl_n = ModelDatasets::cl_n;

//------------------------------------------------------------------------------

ModelFiles::ModelFiles(Hub& hub) : RefHub(hub) {
  setCheckable(true);
  setNumbered(2);
}

cl_n ModelFiles::cols() const {
  return cl_n(1);
}

rw_n ModelFiles::rows() const {
  return rw_n(hub.numFiles());
}

l_qt::var ModelFiles::cell(rw_n rw, cl_n) const {
  return l_qt::var(hub.fileName(rw));
}

ModelFiles::ref ModelFiles::check(rw_n row, bool on) {
  hub.activateFile(row, on); RTHIS
}

bool ModelFiles::isChecked(rw_n row) const {
  return hub.isFileActive(row);
}

//------------------------------------------------------------------------------

ModelDatasets::ModelDatasets(Hub& hub) : RefHub(hub) {
  setNumbered(4);
}

cl_n ModelDatasets::cols() const {
  return cl_n(1 + metaCols.size());
}

rw_n ModelDatasets::rows() const {
  return rw_n(hub.numSets());
}

str ModelDatasets::head(cl_n cl) const {
  if (0 == cl)
    return base::head(cl);

  auto meta = hub.meta();
  if (meta)
    return meta->dict->keys.at(metaCols.at(cl));

  return str::null;
}

l_qt::var ModelDatasets::cell(rw_n rw, cl_n cl) const {
  if (0 == cl)
    return hub.tagAt(rw);

  uint mi = metaCols.at(cl);
  return l_qt::var(hub.setAt(rw).meta()->vals.at(mi));
}

//------------------------------------------------------------------------------

ModelMetadata::ModelMetadata(Hub& hub) : RefHub(hub) {
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
