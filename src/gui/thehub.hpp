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
#include "acts.hpp"
#include "refhub.hpp"
#include "models.hpp"
#include <core/session.hpp>
#include <lib/dev/defs.hpp>
#include <lib/dev/io/path.hpp>
#include <lib/qt/hub.hpp>

/* Note that since both l_qt::Hub and gui::Hub are Q_OBJECT, their base file
 * names (hub & thehub) *must* differ, because that's how MOC operates: on base
 * file names */

namespace gui {
//------------------------------------------------------------------------------

struct Win;

dcl_reimpl2_(Hub, l_qt::Hub, core::Session)
  ref_(Win,  win);
  atr_(Acts, acts);

  ptr_(ModelFiles,    modelFiles);
  ptr_(ModelDatasets, modelDatasets);
  ptr_(ModelMetadata, modelMetadata);

  Hub(Win&);
 ~Hub();

  voi_mut_(sessionClear, ())          emits;
  set_(sessionLoad, (l_io::path)) emits may_err;
  mth_(void, sessionSave, (l_io::path)) may_err;

  mth_(uint, numFiles, ())        RET_(base::files->size())
  mth_(str,  fileName, (uint i))  RET_(base::files->at(i)->name)

  voi_mut_(addFiles,   (l_io::path_vec::rc))  emits;
  voi_mut_(addFiles,   ())                    emits;
  voi_mut_(remFilesAt, (uint_vec::rc))        emits;

  set_(activateFileAt, (uint, bool)) emits;
  using base::isActiveFileAt;
  set_(selectFileAt, (int))       emits;

  set_(activateDatasetAt, (uint, bool)) emits;
  using base::isActiveDatasetAt;
  set_(selectDatasetAt, (int))    emits;

  set_(corrEnable, (bool))        emits;
  set_(corrRem,    ())            emits;

  bol_(corrEnabled, ())           RET_(base::corrEnabled)
  mth_(str, corrName, ())         RET_(base::corrFile ? base::corrFile->name : str::null)

  set_(collectDatasetsFromFiles, (uint_vec::rc, l::pint));
  set_(collectDatasetsFromFiles, (uint_vec::rc));
  set_(groupDatasetsBy,          (l::pint));

  mth_(uint,    numSets,   ())    RET_(base::collectedDatasets.size())
  mth_(l::pint, groupedBy, ())    RET_(base::groupedBy)

  mth_(core::data::CombinedSet::rc, setAt, (uint i)) RET_(*base::collectedDatasets.at(i))
  mth_(strc,                        tagAt, (uint i)) RET_(base::collectedDatasetsTags.at(i))

  mth_(core::data::FilesMetaDict::sh, dict, ()) RET_(files->dict)
  mth_(uint, dictSize, ());
  mth_(str,  dictKey,     (uint));
  mth_(bool, dictChecked, (uint));
  set_(dictCheck, (uint, bool));

signals:
  void sigFiles(core::data::Files::sh) const; // a new set of files <<<<< TODO here I am
  void sigFilesActive() const;    // changed active files

  void sigFileSelected(core::data::File::sh) const; // file selected (or not)

  void sigCorr() const;           // add/rem/on/off correction file

  void sigDatasetsReset() const;  // a major change in datasets
  void sigDatasetsActive() const; // changed active datasets

  void sigDatasetSelected(core::data::CombinedSet::sh) const; // dataset selected (or not)

public:
  template <typename Signal, typename Lambda>
  void onSignal(Signal signal, Lambda slot) {
    QObject::connect(this, signal, slot);
  }

#define DCL_HUB_SIGNAL_ETC(name)      \
private:                              \
  voi_(emit##name, ());               \
public:                               \
  template <typename Lambda> void onSig##name(Lambda slot) { \
    onSignal(&Hub::sig##name, slot);  \
  }

#define DCL_HUB_SIGNAL_ETC2(name, par)  \
private:                                \
  voi_(emit##name, (par p));            \
public:                                 \
  template <typename Lambda> void onSig##name(Lambda slot) { \
    onSignal(&Hub::sig##name, slot);  \
  }

  DCL_HUB_SIGNAL_ETC2(Files, core::data::Files::sh)
  DCL_HUB_SIGNAL_ETC(FilesActive)
  DCL_HUB_SIGNAL_ETC2(FileSelected, core::data::File::sh)

  DCL_HUB_SIGNAL_ETC(Corr)

  DCL_HUB_SIGNAL_ETC(DatasetsReset)
  DCL_HUB_SIGNAL_ETC(DatasetsActive)
  DCL_HUB_SIGNAL_ETC2(DatasetSelected, core::data::CombinedSet::sh)

private:
  Q_OBJECT
dcl_end

//------------------------------------------------------------------------------
}
// eof
