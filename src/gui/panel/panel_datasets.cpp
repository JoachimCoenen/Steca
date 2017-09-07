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

dcl_sub2_(ViewDatasets, RefHub, l_qt::lst_view)
  ViewDatasets(Hub&);

protected:
  void selectionChanged(QItemSelection const&, QItemSelection const&);
dcl_end

ViewDatasets::ViewDatasets(Hub& hub) : RefHub(hub) {
  hub.onSigDatasetsReset([this]() {
// TODO    selectRows({});
//    selUpdateOUT();
  });
}

void ViewDatasets::selectionChanged(QItemSelection const& selected,
                                    QItemSelection const& deselected) {
  base::selectionChanged(selected, deselected);
//  selUpdateOUT();
}

//------------------------------------------------------------------------------

PanelDatasets::PanelDatasets(Hub& hub) : base(""), view(nullptr) {
  auto tabs = new l_qt::tabs;
  vb.add(tabs);
  tabs->addTab((tab = new Panel()), "Datasets");

  ModelDatasets const *model = hub.modelDatasets;

  tab->vb.add((view = new ViewDatasets(hub)));
  view->setModel(model);

  auto&& h = tab->vb.hb();
  h.add(mut(*model).makeTriChk(str::null));
  h.addStretch();
  h.add(new l_qt::lbl("Combine"));

  auto spin = new l_qt::spinPint();
  h.add(spin);

  spin->min(1);
  connect(spin, &l_qt::spinPint::valChg, [model](l::pint val) {
    mut(*model).groupBy(val);
  });

  model->onSignalReset([spin, model](){
    spin->setValue(int(model->groupedBy));
  });
}

//------------------------------------------------------------------------------
}
// eof
