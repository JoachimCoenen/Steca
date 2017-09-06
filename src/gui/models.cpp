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

  hub.onSigFiles([this](core::data::Files::sh sh) {
    files = sh;
    signalReset();
    base::updateState(); // TODO needed?
  });
}

cl_n ModelFiles::cols() const {
  return cl_n(1);
}

rw_n ModelFiles::rows() const {
  return rw_n(files ? files->size() : 0);
}

str ModelFiles::head(cl_n cl) const {
  if (0 == cl)
    return "File";
  return str::null;
}

l_qt::var ModelFiles::cell(rw_n rw, cl_n cl) const {
  EXPECT_(files)
  if (0 == cl)
    return l_qt::var(at(rw)->src->path.filename());
  return str::null;
}

ModelFiles::ref ModelFiles::check(rw_n rw, bool on) {
  hub.activateFileAt(rw, on);
  RTHIS
}

bool ModelFiles::isChecked(rw_n rw) const {
  return at(rw)->isActive;
  EXPECT_(files)
}

core::data::File::sh ModelFiles::at(rw_n rw) const {
  return files->at(rw);
}

//------------------------------------------------------------------------------

ModelDatasets::ModelDatasets(Hub& hub) : RefHub(hub) {
  setCheckable(true);
  setNumbered(4);
}

cl_n ModelDatasets::cols() const {
  // allow 1 empty column even if there are no metacols (looks better)
  return cl_n(numLeadCols() + l::max(1u, metaCols.size()));
}

rw_n ModelDatasets::rows() const {
  return rw_n(0); // TODO rw_n(hub.numSets());
}

str ModelDatasets::head(cl_n cl) const {
  if (0 == cl)
    return "F#";
  if (1 == cl && grouped())
    return "#-#";

  EXPECT_(cl >= numLeadCols())
  uint i = cl - numLeadCols();
  if (i < metaCols.size())
    return hub.dictKey(i);
  return str::null;
}

l_qt::var ModelDatasets::cell(rw_n rw, cl_n cl) const {
  if (0 == cl)
    return "?"; // TODO look up in files l_qt::var(hub.setAt(rw).first()->idx->val);
  if (1 == cl && grouped())
    return "!"; // TODO hub.tagAt(rw);

  EXPECT_(cl >= numLeadCols())
  uint i = cl - numLeadCols();
  if (i < metaCols.size())
    return "."; // TODO l_qt::var(hub.setAt(rw).meta(hub.dict())->vals.valAt(metaCols.at(i))); //TODO
  return l_qt::var();
}

bool ModelDatasets::grouped() const {
  return false; // TODO 1 < hub.groupedBy();
}

uint ModelDatasets::numLeadCols() const {
  return grouped() ? 2 : 1;
}

void ModelDatasets::fixColumns(l_qt::lst_view& view) const {
  base::fixColumns(view);
  view.fixColWidth(cl_n(0), oWidth(view, 3));
}

bool ModelDatasets::rightAlign(cl_n cl) const {
  return 0 == cl;
}

ModelDatasets::ref ModelDatasets::check(rw_n rw, bool on) {
  // TODO
//  hub.activateDatasetAt(rw, on);
//  base::updateState();
  RTHIS
}

bool ModelDatasets::isChecked(rw_n rw) const {
  return false; // TODO hub.isActiveDatasetAt(rw);
}

//------------------------------------------------------------------------------

ModelMetadata::ModelMetadata(Hub& hub) : RefHub(hub) {
  setCheckable(true);
  hub.onSigDatasetSelected([this](core::data::CombinedSet::sh sh) {
    dataset = sh;
    signalReset();
  });
}

cl_n ModelMetadata::cols() const {
  return cl_n(2);
}

rw_n ModelMetadata::rows() const {
  return dataset ? rw_n(hub.dictSize()) : rw_n(0);
}

str ModelMetadata::head(cl_n cl) const {
  if (0 == cl)
    return "Tag";
  if (1 == cl)
    return "Value";
  return str::null;
}

l_qt::var ModelMetadata::cell(rw_n rw, cl_n cl) const {
  if (!dataset)
    return l_qt::var();

  EXPECT_(rw < hub.dictSize());
  auto&& key = hub.dictKey(rw);

  switch (cl) {
  case clTAG:
    return l_qt::var(key);
  case clVAL: {
    auto&& meta = dataset->meta(hub.dict());
    auto&& idx = meta->dict->safeIndex(key);
    if (idx < 0)
      return l_qt::var();
    return l_qt::var(meta->vals.valAt(uint(idx)));
  }
  default:
    return l_qt::var();
  }
}

ModelMetadata::ref ModelMetadata::check(rw_n rw, bool on) {
  hub.dictCheck(rw, on);
  base::updateState();
  RTHIS
}

bool ModelMetadata::isChecked(rw_n rw) const {
  return hub.dictChecked(rw);
}

//------------------------------------------------------------------------------
}
// eof
