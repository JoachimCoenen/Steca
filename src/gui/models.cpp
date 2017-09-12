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
#include <lib/qt/font.hpp>
#include <lib/qt/lst.hpp>

namespace gui {
//------------------------------------------------------------------------------

using rw_n = ModelDatasets::rw_n;
using cl_n = ModelDatasets::cl_n;

//------------------------------------------------------------------------------

Model::Model(Hub& hub) : RefHub(hub) {
  hub.onSigFiles([this](Files::sh sh) {
    if (files != sh) {
      if ((files = sh))
        dict = files->dict;
      else
        dict.drop();

      gotFiles();
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
  if (0 == cl)
    return at(rw)->src->path.filename();
  return str::null;
}

ModelFiles::ref ModelFiles::check(rw_n rw, bool on, bool) {
  hub.activateFileAt(rw, on);
  RTHIS
}

bool ModelFiles::isChecked(rw_n rw) const {
  return at(rw)->isActive;
}

core::data::File::sh ModelFiles::at(rw_n rw) const {
  EXPECT_(files)
  return files->at(rw);
}

//------------------------------------------------------------------------------

ModelDatasets::ModelDatasets(Hub& hub) : base(hub), groupedBy(1) {
  setCheckable(true);
  setNumbered(4);

  hub.onSigMetaChecked([this](KeyBag::sh bag) {
    str_vec ks;
    if (dict && bag)
      for (auto&& key : dict->keys)
        if (bag->contains(key))
          ks.add(key);

    if (ks != metaKeys) {
      metaKeys = ks;
      signalReset();
    }
  });
}

cl_n ModelDatasets::cols() const {
  // allow 1 empty column even if there are no metacols (looks better)
  return cl_n(numLeadCols() + l::max(1u, uint(metaKeys.size())));
}

rw_n ModelDatasets::rows() const {
  return rw_n(sets ? sets->size() : 0);
}

str ModelDatasets::head(cl_n cl) const {
  if (cl < numLeadCols())
    switch (cl) {
    case clFNO: return "F#";
    case clTAG: return "#-#";
    default: NEVER
    }

  EXPECT_(cl >= numLeadCols())
  uint i = cl - numLeadCols();
  if (i < metaKeys.size())
    return metaKeys.at(i);
  return str::null;
}

l_qt::var ModelDatasets::cell(rw_n rw, cl_n cl) const {
  auto&& set = sets->at(rw);

  if (cl < numLeadCols())
    switch (cl) {
    case clFNO: return set->fileNo;
    case clTAG: return set->tag;
    default: NEVER
    }

  EXPECT_(cl >= numLeadCols())
  uint i = cl - numLeadCols();
  if (i < metaKeys.size()) {
    auto&& meta = set->meta();
    int idx = meta->dict->safeIndex(metaKeys.at(i));
    if (0 <= idx)
      return meta->vals.valAt(uint(idx));
  }
  return l_qt::var();
}

void ModelDatasets::fixColumns(l_qt::lst_view& view) const {
  base::fixColumns(view);
  view.fixColWidth(cl_n(0), oWidth(view, 3));
}

bool ModelDatasets::rightAlign(cl_n cl) const {
  return 0 == cl;
}

ModelDatasets::ref ModelDatasets::check(rw_n rw, bool on, bool silent) {
  auto&& isActive = sets->at(rw)->isActive;
  if (isActive != on) {
    mut(isActive) = on;
    if (!silent)
      signalReset();
  }
  RTHIS
}

bool ModelDatasets::isChecked(rw_n rw) const {
  return sets->at(rw)->isActive;
}

void ModelDatasets::groupBy(l::pint by) {
  mut(groupedBy) = by;
  combineSets();
}

void ModelDatasets::emitSetAt(int row) const {
  emit sigSet(
    (sets && 0 <= row && uint(row) < sets->size())
    ? sets->at(uint(row)) : CombinedSet::sh());
}

uint ModelDatasets::numLeadCols() const {
  return 1 < groupedBy ? 2 : 1;
}

void ModelDatasets::combineSets() {
  if (files)
    sets = files->collectDatasets(groupedBy);
  else
    sets.drop();
  signalReset();
}

void ModelDatasets::gotFiles() {
  combineSets();
}

//------------------------------------------------------------------------------

ModelMetadata::ModelMetadata(Hub& hub) : base(hub), checked(new KeyBag) {
  setCheckable(true);
  hub.onSigCombinedSet([this](CombinedSet::sh sh) {
    if (set != sh) {
      set = sh;
      signalReset();
    }
  });
}

cl_n ModelMetadata::cols() const {
  return cl_n(2);
}

rw_n ModelMetadata::rows() const {
  return dict ? rw_n(dict->size()) : rw_n(0);
}

str ModelMetadata::head(cl_n cl) const {
  switch (cl) {
  case clTAG: return "tag";
  case clVAL: return "val";
  default:    NEVER;
  }
}

l_qt::var ModelMetadata::cell(rw_n rw, cl_n cl) const {
  EXPECT_(dict && rw < dict->size());
  auto&& key = dict->key(rw);

  switch (cl) {
  case clTAG:
    return key;
  case clVAL: {
    if (!set)
      return l_qt::var();
    auto&& meta = set->meta();
    auto&& idx  = meta->dict->safeIndex(key);
    return idx < 0 ? l_qt::var() : meta->vals.valAt(uint(idx));
  }
  default:
    NEVER
  }
}

ModelMetadata::ref ModelMetadata::check(rw_n rw, bool on, bool silent) {
  mut(*checked).set(dict->key(rw), on);
  if (!silent)
    signalReset();
  RTHIS
}

bool ModelMetadata::isChecked(rw_n rw) const {
  return checked->contains(dict->key(rw));
}

//------------------------------------------------------------------------------
}
// eof
