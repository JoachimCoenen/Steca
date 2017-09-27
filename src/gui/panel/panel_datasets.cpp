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
#include <lib/qt/inc/defs.inc>
#include <lib/qt/wgt_inc.hpp>
#include "../thehub.hpp"

namespace gui {
//------------------------------------------------------------------------------

dcl_sub_(ViewDatasets, ViewModel<Hub::ModelDatasets>)
  ViewDatasets(Hub&);

private:
  voi_(onSelected, (int));
dcl_end

ViewDatasets::ViewDatasets(Hub& hub) : base(hub, hub.modelDatasets) {}

void ViewDatasets::onSelected(int row) const {
  model->emitSetAt(row);
}

//------------------------------------------------------------------------------

PanelDatasets::PanelDatasets(Hub& hub) : base(""), view(nullptr) {
  auto tabs = new l_qt::tabs;
  vb.add(tabs);
  tabs->addTab(tab = new Panel(), "Datasets");

  tab->vb.add(view = new ViewDatasets(hub));

  auto&& hb = tab->vb.hb();
  hb.add(mut(*view->model).makeTriChk(str::null));
  hb.addStretch();
  hb.add(new l_qt::lbl("Combine"));

  auto&& spin = new l_qt::spinPint();
  hb.add(spin);

  auto&& md = static_cast<Hub::ModelDatasets const*>(view->model);

  spin->min(1);
  connect(spin, &l_qt::spinPint::valChg, [md](l::pint val) {
    mut(*md).groupBy(val);
  });

  md->onSigReset([spin, md](){
    spin->setValue(int(md->groupedBy));
  });
}

//------------------------------------------------------------------------------
}
// eof
