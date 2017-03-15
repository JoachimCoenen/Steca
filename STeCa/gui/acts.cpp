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

  get(QUIT).icon(":/icon/tool_q");

  add(SHOW_FILES,     act::make("Show files",    "Ctrl+1", ":/icon/tool_f"));
  add(SHOW_DATASETS,  act::make("Show datasets", "Ctrl+2", ":/icon/tool_d"));
  add(SHOW_METADATA,  act::make("Show metadata", "Ctrl+3", ":/icon/tool_m"));

  add(FILES_ADD,      act::make("Add files...", "", ":/icon/add"));
  add(FILES_REM,      act::make("Remove selected file", "", ":/icon/rem"));
  add(CORR_ENABLE,    act::make("Enable correction file...", "", ":/icon/check"));
  add(CORR_REM,       act::make("Remove correction file", "", ":/icon/clear"));

  add(DIFF_ZOOM,      act::make("Zoom", ""));
  add(DIFF_ALL_DSETS, act::make("All datasets", ""));
  add(DIFF_FIX_INTEN, act::make("Fixed intensity scale", ""));
}

c::str const Acts::SHOW_FILES("showFiles");
c::str const Acts::SHOW_DATASETS("showDatasets");
c::str const Acts::SHOW_METADATA("showMetadata");

c::str const Acts::FILES_ADD("filesAdd");
c::str const Acts::FILES_REM("filesRem");
c::str const Acts::CORR_ENABLE("corrEnable");
c::str const Acts::CORR_REM("corrRem");

c::str const Acts::DIFF_ZOOM("diffZoom");
c::str const Acts::DIFF_ALL_DSETS("diffAllDsets");
c::str const Acts::DIFF_FIX_INTEN("diffFixInten");

//------------------------------------------------------------------------------
}
// eof
