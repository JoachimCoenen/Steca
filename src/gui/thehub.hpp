/*******************************************************************************
 * Steca2 - StressTextureCalculator ver. 2
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
#include "options.hpp"
#include "refhub.hpp"
#include <core/session.hpp>
#include <core/calc/reflection.hpp>
#include <lib/qt/defs.hpp>
#include <lib/dev/io/path.hpp>
#include <lib/qt/app/hub.hpp>
#include <lib/qt/view/lst.hpp>

/* Note that since both l_qt::Hub and gui::Hub are Q_OBJECT, their base file
 * names (hub & thehub) *must* differ, because that's how MOC operates: on base
 * file names */

namespace gui {
//------------------------------------------------------------------------------

struct Win;

dcl_reimpl2_(Hub, l_qt::Hub, core::Session)
  ref_(Win,  win);
  atr_(Acts, acts);

  dcl_sub2_(HubModel, RefHub, l_qt::lst_model)
    HubModel(Hub&);
  dcl_end

  dcl_sub_(ModelFiles, HubModel)
    friend struct Hub;
    ModelFiles(Hub&);

    mth_(cl_n, cols, ());
    mth_(rw_n, rows, ());

    mth_(str,       head, (cl_n));
    mth_(l_qt::var, cell, (rw_n, cl_n));

    set_(check,     (rw_n, bool, bool));
    bol_(isChecked, (rw_n));

    mth_(core::data::File::shp, at, (rw_n));
  dcl_end

  dcl_sub_(ModelDatasets, HubModel)
    friend struct Hub;
    ModelDatasets(Hub&);

    enum eMdLeadCols { clFNO, clTAG };

    mth_(cl_n, cols, ());
    mth_(rw_n, rows, ());

    mth_(str,       head, (cl_n));
    mth_(l_qt::var, cell, (rw_n, cl_n));

    bol_(rightAlign,  (cl_n));

    set_(check, (rw_n, bool, bool));
    bol_(isChecked, (rw_n));

    atr_(l::pint, groupedBy);
    mut_(groupBy, (l::pint));

    voi_(setSetAt, (int));

  private:
    mth_(uint, numLeadCols, ());
    mut_(combineSets, ());
  dcl_end

  dcl_sub_(ModelMetadata, HubModel)
    friend struct Hub;
    ModelMetadata(Hub&);

    enum { clTAG, clVAL };

    mth_(cl_n, cols, ());
    mth_(rw_n, rows, ());

    mth_(str,       head, (cl_n));
    mth_(l_qt::var, cell, (rw_n, cl_n));

    set_(check, (rw_n, bool, bool));
    bol_(isChecked, (rw_n));

    core::data::KeyBag::shr checked;

  private:
    core::data::CombinedSet::shp set;
  dcl_end

//------------------------------------------------------------------------------

private:
  atr_(dgram_options, dgramOptions);
  atr_(image_options, imageOptions);

  mth_(core::data::Files::rc, currentFiles, ())
    RET_(*base::files)
  mth_(core::data::FilesMetaDict::rc, currentDict, ())
    RET_(base::files->dict())
  core::data::CombinedSets::shr currentSets;
  core::data::CombinedSet::shp  currentSet;
  void setSet(core::data::CombinedSet::shp);
  str_vec currentMetaKeys; // shown metadata

public:
  friend struct ModelFiles;
  friend struct ModelDatasets;
  friend struct ModelMetadata;
  atr_(l::scoped<ModelFiles>,    modelFiles);
  atr_(l::scoped<ModelDatasets>, modelDatasets);
  atr_(l::scoped<ModelMetadata>, modelMetadata);

  Hub(Win&);

  mut_(init, ());

  mut_(sessionClear, ())          emits;
  mut_(sessionLoad, (l_io::path)) emits may_err;
  mth_(void, sessionSave, (l_io::path)) may_err;

  mut_(addFiles,   (l_io::path_vec::rc))  emits;
  mut_(addFiles,   ())                    emits;
  mut_(remFilesAt, (uint_vec::rc))        emits;

  mut_(activateFileAt, (uint, bool)) emits;

  mut_(corrEnable, (bool)) emits;
  mut_(corrRem,    ())     emits;

  mut_(setBg, (core::Ranges::rc)) emits;
  mut_(addBg, (core::Range::rc))  emits;
  mut_(remBg, (core::Range::rc))  emits;

  mut_(setRefl, (Range::rc r)) emits;

  mut_(setNorm, (core::calc::eNorm))   emits;

  voi_(makeDgram, (core::Curve& dgram, core::Curve& bgFitted, core::Curve& bg, core::curve_vec& refls,
                   core::data::CombinedSets::rc, core::data::CombinedSet const*,
                   core::calc::FitParams::rc, bool combined));

  dcl_(SetsInfo)
    atr_(core::data::CombinedSets::shr, sets);
    atr_(core::data::CombinedSet::shp,  set);
    atr_(core::calc::FitParams::shr,    fp);

    SetsInfo(core::data::CombinedSets::shr sets_, core::data::CombinedSet::shp set_,
             core::calc::FitParams::shr fp_)
      : sets(sets_), set(set_), fp(fp_) {}
  dcl_end

  mth_(SetsInfo, setsInfo, ());

private:
  void filesModified();
  void sendSetsInfo();
  void setMetaChecked(core::data::KeyBag::shr);

signals:
  // a new set of combined sets, opt. the selected one, fit params
  void sigSetsInfo(SetsInfo) const;
  // add/rem/on/off correction file
  void sigCorrFileName(str) const;

private:
  mut_(newFiles, ());

#define DCL_HUB_SIG_ETC(name, par)  \
private:                            \
  voi_(emit##name, (par p)) {       \
    emit sig##name(p);              \
  }                                 \
public:                             \
  template <typename Lambda> void onSig##name(Lambda slot) const { \
    QObject::connect(this, &Hub::sig##name, slot);                 \
  }

  DCL_HUB_SIG_ETC(SetsInfo,     SetsInfo)
  DCL_HUB_SIG_ETC(CorrFileName, str)

private:
  Q_OBJECT
dcl_end

//------------------------------------------------------------------------------
}
// eof DOCS
