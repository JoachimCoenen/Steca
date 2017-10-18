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

namespace gui {
//------------------------------------------------------------------------------

dcl_sub_(ViewFile, Panel)
  ViewFile();
  mut_(setInfo, (core::data::File const*));
  ptr_(l_qt::lbl, lbl);
dcl_end

ViewFile::ViewFile() : lbl(new l_qt::lbl()) {
  vb().add(mutp(lbl));
}

void ViewFile::setInfo(core::data::File const* file) {
  mut(*lbl).text(file ? file->src->comment : str::null);
}

//------------------------------------------------------------------------------

dcl_sub_(ViewFiles, ViewModel<Hub::ModelFiles>)
  ViewFiles(Hub&, ViewFile&);

private:
  voi_(onSelected, (int));
  bol_(onKey, (int));

  voi_(removeSelected, ());
  ViewFile& viewFile;
  l_qt::act& actRem;
dcl_end

ViewFiles::ViewFiles(Hub& hub, ViewFile& viewFile_)
: base(hub.modelFiles), viewFile(viewFile_)
, actRem(hub.acts.get(hub.acts.FILES_REM)) {
  actRem.onTrigger([this]() {
    removeSelected();
  });
}

void ViewFiles::onSelected(int row) const {
    core::data::File const* file = nullptr;
    if (0 <= row && model && uint(row) < model->rows())
      file = model->at(rw_n(l::to_uint(row))).ptr();

    viewFile.setInfo(file);
    actRem.setEnabled(file);
}

bool ViewFiles::onKey(int key) const {
  switch (key) {
  case Qt::Key_Delete:
    removeSelected();
    return true;
  default:
    return false;
  }
}

void ViewFiles::removeSelected() const {
  auto row = currentRow();
  if (0 <= row)
    hub.remFilesAt({l::to_u(row)});
}

//------------------------------------------------------------------------------

PanelFiles::PanelFiles(Hub& hub) : base(""), view(nullptr) {
  auto tabs = new l_qt::tabs;
  vb().add(tabs);

  auto& a = hub.acts;

  auto&& p = new l_qt::panel;
  auto&& hb = p->hb();

  auto&& btnAdd = btn(a.get(a.FILES_ADD));
  auto&& btnRem = btn(a.get(a.FILES_REM));

  hb.margin(0).add(btnAdd).add(btnRem);
  tabs->addTab(tab = new Panel(), "Files", p);

  auto&& vf = new ViewFile();
  tab->vb().add(vf);
  tab->vb().add(view = new ViewFiles(hub, *vf));

  tab->vb().add(mut(*view->model).makeTriChk(str::null));
  tab->vb().add("Correction file");

  auto&& h = tab->vb().hb();
  auto&& edit = new l_qt::edit(); edit->ro(true);
  h.add(edit);

  hub.onSigCorrFileName([edit](str name) {
    edit->text(name);
  });

  h.add(btn(a.get(a.CORR_ENABLE)));
  h.add(btn(a.get(a.CORR_REM)));
}

//------------------------------------------------------------------------------
}
// eof
