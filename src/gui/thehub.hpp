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

  set_(sessionClear, ())          emits;
  set_(sessionLoad, (l_io::path)) emits may_err;
  mth_(void, sessionSave, (l_io::path)) may_err;

  mth_(uint, numFiles, ())        RET_(base::files.size())
  mth_(str,  fileName, (uint i))  RET_(base::files.at(i)->name)

  set_(addFiles, (str_vec::rc))   emits;
  set_(addFiles, ())              emits;
  set_(remFile,  (uint))          emits;

  set_(activateFile,(uint, bool)) emits;
  using base::isFileActive;

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

  mth_(core::data::Meta::sh, meta, ());

signals:
  void sigResetFiles();     // a major change in files
  void sigCorr();           // changed correction file, on/off
  void sigFilesActive();    // changed active files
  void sigResetDatasets();  // a major change in datasets

public:
  template <typename Signal, typename Lambda>
  void onSignal(Signal signal, Lambda slot) {
    QObject::connect(this, signal, slot);
  }

#define DCL_HUB_SIGNAL_ETC(name)      \
private:                              \
  set_(emit##name, ());               \
public:                               \
  template <typename Lambda> void onSig##name(Lambda slot) { \
    onSignal(&Hub::sig##name, slot);  \
  }

  DCL_HUB_SIGNAL_ETC(ResetFiles)
  DCL_HUB_SIGNAL_ETC(Corr)
  DCL_HUB_SIGNAL_ETC(FilesActive)
  DCL_HUB_SIGNAL_ETC(ResetDatasets)

private:
  Q_OBJECT
dcl_end

//------------------------------------------------------------------------------
}
// eof