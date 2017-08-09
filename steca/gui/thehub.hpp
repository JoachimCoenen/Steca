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
#include <dev_lib/defs.hpp>
#include <dev_lib/io/path.hpp>
#include <qt_lib/hub.hpp>

/* Note that since both l_qt::Hub and gui::Hub are Q_OBJECT, their base file
 * names (hub & thehub) *must* differ, because that's how MOC operates: on base
 * file names */

namespace gui {
//------------------------------------------------------------------------------

struct Win;

dcl_reimpl2_(Hub, l_qt::Hub, core::Session)
#ifdef DEVELOPMENT
  using base::addFile;
  void emitReset() {
    emit sigReset();
  }
#endif

  ref_(Win,  win);
  atr_(Acts, acts);

  ptr_(ModelFiles,    modelFiles);
  ptr_(ModelDatasets, modelDatasets);
  ptr_(ModelMetadata, modelMetadata);

  Hub(Win&);
 ~Hub();

  set_(sessionClear, ())          emits(sigReset);
  set_(sessionLoad, (l_io::path)) emits(sigReset)   may_err;
  mth_(void, sessionSave, (l_io::path))             may_err;

  mth_(uint, numFiles, ())        RET_(base::files.size())
  mth_(str,  fileName, (uint i))  RET_(base::files.at(i)->name)

  set_(addFiles, ())              emits(sigReset);
  set_(remFile,  (uint))          emits(sigReset);

  set_(activateFile,(uint, bool)) emits(sigReset);
  using base::isFileActive;

  set_(corrEnable, (bool))        emits(sigCorr);
  set_(corrRem,    ())            emits(sigCorr);

signals:
  void sigReset();        // major change in data
  void sigCorr();         // changed correction - file, on/off
  void sigFilesActive();  // changed active files

public:
  template <typename Signal, typename Lambda>
  void onSignal(Signal signal, Lambda slot) {
    QObject::connect(this, signal, slot);
  }

#define DCL_HUB_SIGNAL_HANDLER(name) \
  template <typename Lambda> void onSig##name(Lambda slot) { \
    onSignal(&Hub::sig##name, slot); \
  }

  DCL_HUB_SIGNAL_HANDLER(Reset)
  DCL_HUB_SIGNAL_HANDLER(Corr)
  DCL_HUB_SIGNAL_HANDLER(FilesActive)

private:
  Q_OBJECT
dcl_end

//------------------------------------------------------------------------------
}
// eof
