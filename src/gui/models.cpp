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

Model::Model(Hub& hub) : RefHub(hub) {
  hub.onSigFiles([this](core::data::Files::sh sh) {
    if (files != sh) {
      files = sh;
      newFiles();
      signalReset();
    }
  });
}

//------------------------------------------------------------------------------

ModelFiles::ModelFiles(Hub& hub) : base(hub) {
  setCheckable(true);
  setNumbered(2);
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
    return at(rw)->src->path.filename();
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

ModelDatasets::ModelDatasets(Hub& hub) : base(hub), groupedBy(1) {
  setCheckable(true);
  setNumbered(4);
}

void ModelDatasets::newFiles() {
  combineSets();
}

cl_n ModelDatasets::cols() const {
  // allow 1 empty column even if there are no metacols (looks better)
  return cl_n(numLeadCols() + l::max(1u, metaCols.size()));
}

rw_n ModelDatasets::rows() const {
  return rw_n(combinedSets ? combinedSets->size() : 0);
}

str ModelDatasets::head(cl_n cl) const {
  if (cl < numLeadCols())
    switch (cl) {
    case 0: return "F#";
    case 1: return "#-#";
    }

  EXPECT_(cl >= numLeadCols())
  uint i = cl - numLeadCols();
  if (i < metaCols.size())
    return files->dict->key(i);
  return str::null;
}

l_qt::var ModelDatasets::cell(rw_n rw, cl_n cl) const {
  if (cl < numLeadCols())
    switch (cl) {
    case 0: return combinedSets->at(rw)->fileNo;
    case 1: return combinedSets->at(rw)->tag;
    }

  EXPECT_(cl >= numLeadCols())
  uint i = cl - numLeadCols();
  if (i < metaCols.size())
    return combinedSets->at(rw)->meta(files->dict)->vals.valAt(metaCols.at(i));
  return l_qt::var();
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

void ModelDatasets::groupBy(l::pint by) {
  mut(groupedBy) = by;
  combineSets();
}

uint ModelDatasets::numLeadCols() const {
  return 1 < groupedBy ? 2 : 1;
}

void ModelDatasets::combineSets() {
  if (files)
    combinedSets = files->collectDatasets(groupedBy);
  else
    combinedSets.drop();
  signalReset();
}

//------------------------------------------------------------------------------

ModelMetadata::ModelMetadata(Hub& hub) : base(hub), checked() {
  setCheckable(true);
}

void ModelMetadata::newFiles() {
  dict = files->dict;
}

cl_n ModelMetadata::cols() const {
  return cl_n(2);
}

rw_n ModelMetadata::rows() const {
  return dataset ? rw_n(files->dict->size()) : rw_n(0);
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

  EXPECT_(rw < files->dict->size());
  auto&& key = files->dict->key(rw);

  switch (cl) {
  case clTAG:
    return key;
  case clVAL: {
//    auto&& meta = dataset->meta(hub.dict());
//    auto&& idx = meta->dict->safeIndex(key);
//    if (idx < 0)
      return l_qt::var();
//    return meta->vals.valAt(uint(idx));
  }
  default:
    return l_qt::var();
  }
}

ModelMetadata::ref ModelMetadata::check(rw_n rw, bool on) {
  auto&& key = dict->key(rw);
  if (on)
    mut(checked).add(key);
  else
    checked.remove(key);
  base::updateState();
  RTHIS
}

bool ModelMetadata::isChecked(rw_n rw) const {
  return checked.contains(dict->key(rw));
}

//------------------------------------------------------------------------------
}
// eof
