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
#include <lib/qt/win.hpp>
#include "thehub.hpp"
#include <QNetworkAccessManager>

namespace gui {
//------------------------------------------------------------------------------

struct PanelFiles;
struct PanelDatasets;
struct PanelSetup;
struct PanelImage;
struct PanelDiffractogram;
struct PanelMetadata;

dcl_sub_(Win, l_qt::win)
  Win();
  Hub hub;

  mut_(init, ());
  mth_(l_qt::acts const&, getActs, ());

  mut_(about, ());
  mut_(onFirstShow, ());
  bol_mut_(onClose, ());

  voi_(checkUpdate, (bool completeReport = true));

  PanelFiles         *panelFiles;
  PanelDatasets      *panelDatasets;
  PanelSetup         *panelSetup;
  PanelImage         *panelImage;
  PanelDiffractogram *panelDiffractogram;
  PanelMetadata      *panelMetadata;

private:
  mut_(checkActions, ());
  mutable QNetworkAccessManager netMan;
dcl_end

//------------------------------------------------------------------------------
}
// eof
