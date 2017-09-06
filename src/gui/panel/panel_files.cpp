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
#include <lib/qt/inc/defs.inc>
#include <lib/qt/wgt_inc.hpp>
#include "../thehub.hpp"
#include <QKeyEvent>

namespace gui {
//------------------------------------------------------------------------------

dcl_sub_(ViewFile, Panel)
  ViewFile();
  voi_mut_(setInfo, (core::data::File const*));
  ptr_(l_qt::lbl, lbl);
dcl_end

ViewFile::ViewFile() : lbl(new l_qt::lbl()) {
  vb.add(mutp(lbl));
}

void ViewFile::setInfo(core::data::File const* file) {
  mutp(lbl)->text(file ? file->src->comment : str::null);
}

//------------------------------------------------------------------------------

dcl_sub2_(ViewFiles, RefHub, l_qt::lst_view)
  ViewFiles(Hub&, ViewFile&);

  void removeSelected();
  void collectDatasetsOUT();

private:
  ViewFile& viewFile;
  l_qt::act& actRem;
  void keyPressEvent(QKeyEvent*);
  void selectionChanged(QItemSelection const&, QItemSelection const&);
dcl_end

ViewFiles::ViewFiles(Hub& hub, ViewFile& viewFile_) : RefHub(hub)
, viewFile(viewFile_) , actRem(hub.acts.get(hub.acts.FILES_REM)) {
  setModel(hub.modelFiles);

  actRem.onTrigger([this]() {
    removeSelected();
  });
}

void ViewFiles::removeSelected() {
  auto row = currentRow();
  if (0 <= row)
    hub.remFilesAt({l::to_u(row)});
}

void ViewFiles::collectDatasetsOUT() {
  hub.collectDatasetsFromFilesOUT(checkedRows());
}

void ViewFiles::keyPressEvent(QKeyEvent* e) {
  switch (e->key()) {
  case Qt::Key_Delete:
    removeSelected();
    return;
  default:
    base::keyPressEvent(e);
  }
}

void ViewFiles::selectionChanged(QItemSelection const& selected,
                                 QItemSelection const& deselected) {
  base::selectionChanged(selected, deselected);
  int i = currentRow();
  if (0 <=i && model && uint(i) < model->rows()) {
    EXPECT_(dynamic_cast<ModelFiles const*>(model))
    auto m = static_cast<ModelFiles const*>(model);
    viewFile.setInfo(m->at(rw_n(l::to_u(i))).ptr());
  } else
    viewFile.setInfo(nullptr);
}

//------------------------------------------------------------------------------

PanelFiles::PanelFiles(Hub& hub) : base(""), view(nullptr) {
  auto tabs = new l_qt::tabs;
  vb.add(tabs);

  auto& a = hub.acts;

  auto&& p = new l_qt::panel;
  auto&& hb = p->hb();

  auto&& btnAdd = new l_qt::actbtn(a.get(a.FILES_ADD));
  auto&& btnRem = new l_qt::actbtn(a.get(a.FILES_REM));

  hb.margin(0).add(btnAdd).add(btnRem);
  tabs->addTab((tab = new Panel()), "Files", p);

  auto&& vf = new ViewFile();
  tab->vb.add(vf);
  tab->vb.add((view = new ViewFiles(hub, *vf)));

  tab->vb.add(mutp(hub.modelFiles)->makeTriChk(str::null));
  tab->vb.add(new l_qt::lbl("Correction file"));

  auto&& h = tab->vb.hb();
  auto&& edit = new l_qt::edit(); edit->ro(true);
  h.add(edit);

  hub.onSigCorr([this, edit, &hub]() {
    edit->text(hub.corrName());
  });

  h.add(new l_qt::actbtn(a.get(a.CORR_ENABLE)));
  h.add(new l_qt::actbtn(a.get(a.CORR_REM)));
}

//------------------------------------------------------------------------------
}
// eof
