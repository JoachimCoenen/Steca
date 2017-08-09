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
#include "../thehub.hpp"
#include <qt_lib/wgt_inc.hpp>
#include <dev_lib/defs.inc>

namespace gui {
//------------------------------------------------------------------------------

dcl_sub2_(ViewFiles, RefHub, l_qt::lst_view)
  ViewFiles(Hub&);

//protected:
//  using Model = models::FilesModel;
//  Model* model() const { return static_cast<Model*>(super::model()); }

//  void selectionChanged(QItemSelection const&, QItemSelection const&);
  void removeSelected();
  void recollect();
dcl_end

ViewFiles::ViewFiles(Hub& hub) : RefHub(hub) {
  hub.acts.get(hub.acts.FILES_REM).onTrigger([this]() {
    removeSelected();
  });
//  header()->hide();

//  connect(hub_.actions.remFile, &QAction::triggered,
//          [this]() { removeSelected(); });

//  onSigFilesChanged([this]() {
//    selectRows({});
//    recollect();
//  });

//  onSigFilesSelected([this]() {
//    selectRows(hub_.collectedFromFiles());
//  });
}

//void ViewFiles::selectionChanged(QItemSelection const& selected,
//                                 QItemSelection const& deselected) {
//  super::selectionChanged(selected, deselected);
//  recollect();
//}

void ViewFiles::removeSelected() {
  auto row = selectedRow();
  if (row < 0)
    return;

  hub.remFile(l::to_u(row));

  auto num = hub.numFiles();
  if (num > 0)
    selectRow(rw_n(l::min(num-1, l::to_uint(row))));
//  recollect();
}

void ViewFiles::recollect() {
//  uint_vec rows;
//  for (auto& index : selectionModel()->selectedRows())
//    if (index.isValid())
//      rows.append(to_u(index.row()));

//  hub_.collectDatasetsFromFiles(rows);
}

//------------------------------------------------------------------------------

PanelFiles::PanelFiles(Hub& hub_) : base("", hub_), view(nullptr) {
  auto tabs = new l_qt::tabs;
  vb.add(tabs);

  auto &a = hub.acts;

  auto p = new l_qt::panel;
  auto &hb = p->hb();

  auto btnAdd = new l_qt::actbtn(a.get(a.FILES_ADD));
  auto btnRem = new l_qt::actbtn(a.get(a.FILES_REM));

  hb.margin(0).add(btnAdd).add(btnRem);
  tabs->addTab((tab = new Panel(hub)), "Files", p);

  tab->vb.add((view = new ViewFiles(hub)));
  view->showHeader(true);
  view->setModel(hub.modelFiles);

  tab->vb.add(new l_qt::lbl("Correction file"));
  auto &h = tab->vb.hb();
  h.add(new l_qt::edit());
  h.add(new l_qt::actbtn(a.get(a.CORR_ENABLE)));
  h.add(new l_qt::actbtn(a.get(a.CORR_REM)));
}

//------------------------------------------------------------------------------
}
// eof
