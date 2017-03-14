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

#include "acts.hpp"
#include "win.hpp"
#include "win.hpp"
#include "panel/inc.inc"
#include <c2/c/c_cpp>
#include <QMenu>

namespace gui {
//------------------------------------------------------------------------------

template <typename L>
void f() { L(); }

Acts::Acts(Win& w) : base(w) {
  using act = c_qt::act;

  add(SHOW_FILES,    act::make("Show files",    "Ctrl+1", ":/icon/tool_f"));
  add(SHOW_DATASETS, act::make("Show datasets", "Ctrl+2", ":/icon/tool_d"));
  add(SHOW_METADATA, act::make("Show metadata", "Ctrl+3", ":/icon/tool_m"));

  get(QUIT).icon(":/icon/tool_q");
}

c::str const Acts::SHOW_FILES("showFiles");
c::str const Acts::SHOW_DATASETS("showDatasets");
c::str const Acts::SHOW_METADATA("showMetadata");

void Acts::setup() {
  auto &w = static_cast<Win&>(base::w);

  get(SHOW_FILES).onToggle([&w](bool on) {
    w.panelFiles->show(on);
  }).check();

  get(SHOW_DATASETS).onToggle([&w](bool on) {
    w.panelDatasets->show(on);
  }).check();

  get(SHOW_METADATA).onToggle([&w](bool on) {
    w.panelMetadata->show(on);
  }).check();
}

//------------------------------------------------------------------------------
}
// eof
