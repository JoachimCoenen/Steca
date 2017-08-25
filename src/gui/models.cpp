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
#include <lib/qt/inc/defs.inc>
#include <lib/qt/lst.hpp>
#include <lib/qt/font.hpp>

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
  hub.activateFile(row, on);
  base::updateState();
  RTHIS
}

bool ModelFiles::isChecked(rw_n row) const {
  return hub.isFileActive(row);
}

//------------------------------------------------------------------------------

ModelDatasets::ModelDatasets(Hub& hub) : RefHub(hub) {
  setCheckable(true);
  setNumbered(4);
}

cl_n ModelDatasets::cols() const {
  // allow an empty column if there are no metacols (looks better)
  return cl_n(numFixedCols() + l::max(1u, metaCols.size()));
}

rw_n ModelDatasets::rows() const {
  return rw_n(hub.numSets());
}

str ModelDatasets::head(cl_n cl) const {
  if (0 == cl)
    return "F#";
  if (1 == cl && grouped())
    return "#-#";

  cl = cl_n(cl - numFixedCols());

  auto meta = hub.meta();
  if (meta && cl < metaCols.size())
    return meta->dict->keys.at(metaCols.at(cl));

  return str::null;
}

l_qt::var ModelDatasets::cell(rw_n rw, cl_n cl) const {
  if (0 == cl)
    return l_qt::var(hub.setAt(rw).first()->file.idx);
  if (1 == cl && grouped())
    return hub.tagAt(rw);

  cl = cl_n(cl - numFixedCols());

  if (cl < metaCols.size())
    return l_qt::var(hub.setAt(rw).meta()->vals.at(metaCols.at(cl)));

  return l_qt::var();
}

bool ModelDatasets::grouped() const {
  return 1 < hub.groupedBy();
}

uint ModelDatasets::numFixedCols() const {
  return grouped() ? 2 : 1;
}

void ModelDatasets::sizeColumns(l_qt::lst_view& view) const {
  base::sizeColumns(view);
  view.fixColWidth(cl_n(0), oWidth(view, 3));
}

bool ModelDatasets::rightAlign(cl_n cl) const {
  return 0 == cl;
}

//------------------------------------------------------------------------------

ModelMetadata::ModelMetadata(Hub& hub) : RefHub(hub) {
  setCheckable(true);
}

cl_n ModelMetadata::cols() const {
  return cl_n(2);
}

rw_n ModelMetadata::rows() const {
  auto meta = hub.meta();
  if (!meta)
    return rw_n(0);
  return rw_n(meta->dict->keys.size());
}

l_qt::var ModelMetadata::cell(rw_n rw, cl_n cl) const {
  EXPECT_(hub.meta())
  auto& dict = *hub.meta()->dict;

  switch (cl) {
  case clTAG:
    return l_qt::var(dict.keys.at(rw));
  case clVAL:
    return l_qt::var(1);
  default:
    return l_qt::var();
  }
}

ModelMetadata::ref ModelMetadata::check(rw_n row, bool on) {
  EXPECT_(hub.meta())
  mut(hub.meta()->dict->checked).setAt(row, on);
  base::updateState();
  RTHIS
}

bool ModelMetadata::isChecked(rw_n row) const {
  EXPECT_(hub.meta())
  return hub.meta()->dict->checked.at(row);
}

//------------------------------------------------------------------------------
}
// eof
