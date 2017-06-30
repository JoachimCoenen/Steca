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
#include <qt_lib/wgt_inc.hpp>
#include "../models.hpp"

namespace gui {
//------------------------------------------------------------------------------

PanelFiles::PanelFiles(Hub& hub_)
: base("", hub_), view(nullptr), model(nullptr) {
  auto tabs = new l_qt::tabs;
  vb.add(tabs);

  auto &a = hub.acts;

  auto p = new l_qt::panel;
  auto &hb = p->hb();

  auto btnAdd = new l_qt::actbtn(a.get(a.FILES_ADD));
  auto btnRem = new l_qt::actbtn(a.get(a.FILES_REM));

  hb.margin(0).add(btnAdd).add(btnRem);
  tabs->addTab((tab = new Panel(hub)), "Files", p);

  tab->vb.add((view = new l_qt::lst_view));
  view->showHeader(true);
  view->set((model = new core_qt::ModelFiles));

  tab->vb.add(new l_qt::lbl("Correction file"));
  auto &h = tab->vb.hb();
  h.add(new l_qt::edit());
  h.add(new l_qt::actbtn(a.get(a.CORR_ENABLE)));
  h.add(new l_qt::actbtn(a.get(a.CORR_REM)));
}

PanelFiles::~PanelFiles() {
  delete view;
  delete model;
}

//------------------------------------------------------------------------------
}
// eof
