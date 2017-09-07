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

  voi_mut_(addFiles,   (l_io::path_vec::rc))  emits;
  voi_mut_(addFiles,   ())                    emits;
  voi_mut_(remFilesAt, (uint_vec::rc))        emits;

  voi_mut_(activateFileAt, (uint, bool)) emits;
  using base::isActiveFileAtOUT;
  set_(selectFileAtOUT, (int))       emits;

  set_(activateDatasetAtOUT, (uint, bool)) emits;
  using base::isActiveDatasetAtOUT;
  set_(selectDatasetAtOUT, (int))    emits;

  set_(corrEnable, (bool))        emits;
  set_(corrRem,    ())            emits;

  bol_(corrEnabled, ())           RET_(base::corrEnabled)
  mth_(str, corrName, ())         RET_(base::corrFile ? base::corrFile->src->path.filename() : str::null)

  set_(collectDatasetsFromFilesOUT, (uint_vec::rc, l::pint));
  set_(collectDatasetsFromFilesOUT, (uint_vec::rc));
  set_(groupDatasetsByOUT,          (l::pint));

  mth_(uint,    numSetsOUT,   ())    RET_(base::collectedDatasetsOUT.size())
  mth_(l::pint, groupedByOUT, ())    RET_(base::groupedByOUT)

  mth_(core::data::CombinedSet::rc, setAtOUT, (uint i)) RET_(*base::collectedDatasetsOUT.at(i))
  mth_(strc,                        tagAtOUT, (uint i)) RET_(base::collectedDatasetsTagsOUT.at(i))

  mth_(uint, dictSizeOUT, ());
  mth_(str,  dictKeyOUT,     (uint));
  mth_(bool, dictCheckedOUT, (uint));
  set_(dictCheckOUT, (uint, bool));

signals:
  void sigFiles(core::data::Files::sh) const; // a new set of files <<<<< TODO here I am
  void sigFilesActiveOUT() const;    // changed active files

  void sigFileSelectedOUT(core::data::File::sh) const; // file selected (or not)

  void sigCorr() const;           // add/rem/on/off correction file

  void sigDatasetsReset() const;  // a major change in datasets
  void sigDatasetsActive() const; // changed active datasets

  void sigDatasetSelected(core::data::CombinedSet::sh) const; // dataset selected (or not)

public:
  template <typename Signal, typename Lambda>
  void onSignal(Signal signal, Lambda slot) const {
    QObject::connect(this, signal, slot);
  }

#define DCL_HUB_SIGNAL_ETC(name)      \
private:                              \
  voi_(emit##name, ());               \
public:                               \
  template <typename Lambda> void onSig##name(Lambda slot) const { \
    onSignal(&Hub::sig##name, slot);  \
  }

#define DCL_HUB_SIGNAL_ETC2(name, par)  \
private:                                \
  voi_(emit##name, (par p));            \
public:                                 \
  template <typename Lambda> void onSig##name(Lambda slot) const { \
    onSignal(&Hub::sig##name, slot);  \
  }

  DCL_HUB_SIGNAL_ETC2(Files, core::data::Files::sh)
  DCL_HUB_SIGNAL_ETC(FilesActiveOUT)
  DCL_HUB_SIGNAL_ETC2(FileSelectedOUT, core::data::File::sh)

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
