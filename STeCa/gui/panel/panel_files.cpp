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

#include "panel_files.hpp"
#include "../hub.hpp"
#include <c2/gui_qt/inc.inc>
#include <core_qt/models.hpp>

namespace gui {
//------------------------------------------------------------------------------

PanelFiles::PanelFiles(Hub& hub_)
: base("", hub_), view(nullptr), model(nullptr) {
  auto &a = hub.acts;

  {
    auto &h = vb.hb();
    h.add(new c_qt::lbl("Files"));
    h.addStretch();
    h.add(new c_qt::actbtn(a.get(a.FILES_ADD)));
    h.add(new c_qt::actbtn(a.get(a.FILES_REM)));
  }

  {
    vb.add((view = new c_qt::lst_view));
    view->showHeader(true);
    view->set((model = new core_qt::ModelFiles));
  }

  {
    vb.add(new c_qt::lbl("Correction file"));
    auto &h = vb.hb();
    h.add(new c_qt::edit());
    h.add(new c_qt::actbtn(a.get(a.CORR_ENABLE)));
    h.add(new c_qt::actbtn(a.get(a.CORR_REM)));
  }
}

PanelFiles::~PanelFiles() {
  delete view;
  delete model;
}

//------------------------------------------------------------------------------
}
// eof
