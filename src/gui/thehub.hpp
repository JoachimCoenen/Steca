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
#include "options.hpp"
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

  friend struct ModelFiles;
  friend struct ModelDatasets;
  friend struct ModelMetadata;
  ptr_(ModelFiles,    modelFiles);
  ptr_(ModelDatasets, modelDatasets);
  ptr_(ModelMetadata, modelMetadata);

  Hub(Win&);
 ~Hub();

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

//  bol_(corrEnabled, ()); // TODO out - signal
  mth_(str, corrName, ()); // TODO out - signal

  mut_(setBg, (core::Ranges::rc)) emits;
  mut_(addBg, (core::Range::rc))  emits;
  mut_(remBg, (core::Range::rc))  emits;

  mut_(setRefl, (Range::rc r)) emits;

  mut_(setNorm, (core::eNorm))   emits;

  atr_(dgram_options, dgramOptions);
  atr_(image_options, imageOptions);

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

private:
  void sendSetsInfo(core::data::CombinedSets::shr, core::data::CombinedSet::shp); // TODO mth_,
  void sendMetaChecked(core::data::KeyBag::shr); // TODO mth_,
  // TODO move models to hub, remove their data, replace with hub's active collection

signals:
  // new set of files
  void sigFiles(core::data::Files::shr) const;
  // a new set of combined sets, opt. the selected one, fit params
  void sigSetsInfo(SetsInfo) const;
  // checked metadata
  void sigMetaChecked(core::data::KeyBag::shr) const;

  // add/rem/on/off correction file
  void sigCorr(core::data::File::shp) const;

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

  DCL_HUB_SIG_ETC(Files,        core::data::Files::shr)
  DCL_HUB_SIG_ETC(SetsInfo,     SetsInfo)
  DCL_HUB_SIG_ETC(MetaChecked,  core::data::KeyBag::shr)

  DCL_HUB_SIG_ETC(Corr,         core::data::File::shp)

private:
  Q_OBJECT
dcl_end

//------------------------------------------------------------------------------
}
// eof
