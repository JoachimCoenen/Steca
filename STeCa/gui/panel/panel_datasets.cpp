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

#include "panel_datasets.hpp"
#include <c2/gui_qt/inc.inc>
#include <core_qt/models.hpp>

namespace gui {
//------------------------------------------------------------------------------

PanelDatasets::PanelDatasets(Hub& hub_)
: base("Datasets", hub_), view(nullptr), model(nullptr) {
  vb.add((view = new c_qt::lst_view));
  auto &h = vb.hb();
  h.addStretch();
  h.add(new c_qt::lbl("Combine"));
  h.add(new c_qt::spin());

  view->showHeader(true);
  view->set((model = new core_qt::ModelDatasets));
}

PanelDatasets::~PanelDatasets() {
  delete view;
  delete model;
}

//------------------------------------------------------------------------------
}
// eof
