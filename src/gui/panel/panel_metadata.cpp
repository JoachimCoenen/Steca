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

#include "panel_metadata.hpp"
#include <lib/qt/inc/defs.inc>
#include <lib/qt/wgt_inc.hpp>
#include "../thehub.hpp"

namespace gui {
//------------------------------------------------------------------------------

dcl_sub2_(ViewMetadata, RefHub, l_qt::lst_view)
  ViewMetadata(Hub&);
dcl_end

ViewMetadata::ViewMetadata(Hub& hub) : RefHub(hub) {}

//------------------------------------------------------------------------------

PanelMetadata::PanelMetadata(Hub& hub) : base("", hub), view(nullptr) {
  auto tabs = new l_qt::tabs;
  vb.add(tabs);
  tabs->addTab((tab = new Panel(hub)), "Metadata");

  tab->vb.add((view = new ViewMetadata(hub)));
  view->setModel(hub.modelMetadata);

  tab->vb.add(mutp(hub.modelMetadata)->makeTriChk(str::null));
}

//------------------------------------------------------------------------------
}
// eof
