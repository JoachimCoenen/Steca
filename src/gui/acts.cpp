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

namespace gui {
//------------------------------------------------------------------------------

template <typename L>
void f() { L(); }

Acts::Acts(Hub& hub, Win& win) : base(win) {
  using act = l_qt::act;

#ifndef Q_OS_OSX
  get(FULL_SCREEN).key("F11").icon(":/icon/full");
#endif

  get(ABOUT).icon(":/icon/tool_a");
  get(QUIT).icon(":/icon/tool_q");

  add(SHOW_FILES,     act::make("Show files",                 "Ctrl+F", ":/icon/tool_f"));
  add(SHOW_DATASETS,  act::make("Show datasets",              "Ctrl+D", ":/icon/tool_d"));
  add(SHOW_METADATA,  act::make("Show metadata",              "Ctrl+M", ":/icon/tool_m"));

  add(SESSION_LOAD,   act::make("Load session",               "",       ":/icon/load"));
  add(SESSION_SAVE,   act::make("Save session",               "",       ":/icon/save"));

  add(FILES_ADD,      act::make("Add files...",               "",       ":/icon/add"));
  add(FILES_REM,      act::make("Remove selected file",       "",       ":/icon/rem"));
  add(CORR_ENABLE,    act::make("Enable correction file...",  "",       ":/icon/check"));
  add(CORR_REM,       act::make("Remove correction file",     "",       ":/icon/clear"));

  add(DIFF_ZOOM,      act::make("Zoom",                       ""));
  add(DIFF_ALL_DSETS, act::make("All datasets",               ""));
  add(DIFF_FIX_INTEN, act::make("Fixed intensity scale",      ""));

  get(FILES_ADD).onTrigger([&hub]() {
    hub.addFiles();
  });

  auto& actCE = get(CORR_ENABLE);
  actCE.onToggle([&hub](bool on) {
    hub.corrEnable(on);
  });

  hub.onSigCorr([&hub, &actCE]() {
    actCE.check(hub.corrEnabled());
  });

  get(CORR_REM).onTrigger([&hub]() {
    hub.corrRem();
  });
}

str const Acts::SHOW_FILES      ("showFiles");
str const Acts::SHOW_DATASETS   ("showDatasets");
str const Acts::SHOW_METADATA   ("showMetadata");

str const Acts::SESSION_LOAD    ("sessionLoad");
str const Acts::SESSION_SAVE    ("sessionSave");

str const Acts::FILES_ADD       ("filesAdd");
str const Acts::FILES_REM       ("filesRem");
str const Acts::CORR_ENABLE     ("corrEnable");
str const Acts::CORR_REM        ("corrRem");

str const Acts::DIFF_ZOOM       ("diffZoom");
str const Acts::DIFF_ALL_DSETS  ("diffAllDsets");
str const Acts::DIFF_FIX_INTEN  ("diffFixInten");

//------------------------------------------------------------------------------
}
// eof
