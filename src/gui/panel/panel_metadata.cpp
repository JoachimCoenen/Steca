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

#include "panel_metadata.hpp"
#include <lib/qt/defs.inc>
#include <lib/qt/wgt_inc.hpp>
#include "../thehub.hpp"
#include "model_view.hpp"

namespace gui {
//------------------------------------------------------------------------------

dcl_sub_(ViewMetadata, HubView<Hub::ModelMetadata>)
  ViewMetadata(Hub&);
dcl_end

ViewMetadata::ViewMetadata(Hub& hub) : base(hub, hub.modelMetadata) {}

//------------------------------------------------------------------------------

PanelMetadata::PanelMetadata(Hub& hub) : base(""), view(nullptr) {
  auto tabs = new l_qt::tabs;
  vb().add(tabs);
  tabs->addTab(tab = new Panel(), "Metadata");

  tab->vb().add(view = new ViewMetadata(hub));
  tab->vb().add(mut(*view->model).makeTriChk(str::null));
}

//------------------------------------------------------------------------------
}
// eof
