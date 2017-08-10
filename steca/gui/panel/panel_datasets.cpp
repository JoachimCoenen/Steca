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
#include "../thehub.hpp"
#include <qt_lib/wgt_inc.hpp>
#include <dev_lib/defs.inc>

namespace gui {
//------------------------------------------------------------------------------

dcl_sub2_(ViewDatasets, RefHub, l_qt::lst_view)
  ViewDatasets(Hub&);
dcl_end

ViewDatasets::ViewDatasets(Hub& hub) : RefHub(hub) {
  hub.onSigResetDatasets([this]() {
  });
}

//------------------------------------------------------------------------------

PanelDatasets::PanelDatasets(Hub& hub) : base("", hub), view(nullptr) {
  auto tabs = new l_qt::tabs;
  vb.add(tabs);
  tabs->addTab((tab = new Panel(hub)), "Datasets");

  tab->vb.add((view = new ViewDatasets(hub)));
  view->showHeader(true);
  view->setModel(hub.modelDatasets);

  auto &h = tab->vb.hb();
  h.addStretch();
  h.add(new l_qt::lbl("Combine"));

  auto spin = new l_qt::spinPint();
  h.add(spin);

  spin->min(1);
  connect(spin, &l_qt::spinPint::valChg, [&hub](l::pint val) {
    hub.groupDatasetsBy(val);
  });

  hub.onSigResetDatasets([spin, &hub]() {
    spin->setValue(hub.groupedBy());
  });
}

//------------------------------------------------------------------------------
}
// eof
