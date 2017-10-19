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

  add(SHOW_FILES,     act::make("Show files",                 "Ctrl+1", ":/icon/tool_f"));
  add(SHOW_DATASETS,  act::make("Show datasets",              "Ctrl+2", ":/icon/tool_d"));
  add(SHOW_METADATA,  act::make("Show metadata",              "Ctrl+3", ":/icon/tool_m"));

  add(SESSION_LOAD,   act::make("Load session",               "Ctrl+O", ":/icon/load"));
  add(SESSION_SAVE,   act::make("Save session",               "Ctrl+S", ":/icon/save"));

  add(FILES_ADD,      act::make("Add files...",               "Ins",    ":/icon/add"));
  add(FILES_REM,      act::make("Remove selected file",       "Del",    ":/icon/rem"));
  add(CORR_ENABLE,    act::make("Enable correction file...",  "",       ":/icon/check"));
  add(CORR_REM,       act::make("Remove correction file",     "",       ":/icon/clear"));

  add(DIFF_ZOOM,      act::make("Zoom",                       ""));
  add(DIFF_ALL_DSETS, act::make("All datasets",               ""));
  add(DIFF_FIX_INTEN, act::make("Fixed intensity scale",      ""));

  add(IMG_FIX_INTEN,  act::make("Fixed intensity scale",      "",       ":/icon/scale"));
  add(IMG_STEP_ZOOM,  act::make("Zoom in steps",              "",       ":/icon/stepz"));
  add(IMG_SHOW_OVER,  act::make("Show overlay",               "",       ":/icon/crop"));
  add(IMG_SHOW_GAMMA, act::make("Show gamma",                 "",       ":/icon/angle"));

  add(IMG_ROTATE0,    act::make("Rotate 90°",                 "",       ":/icon/rotate0"));
  add(IMG_ROTATE1,    act::make("Rotate 180°",                "",       ":/icon/rotate1"));
  add(IMG_ROTATE2,    act::make("Rotate 270°",                "",       ":/icon/rotate2"));
  add(IMG_ROTATE3,    act::make("Rotate 0°",                  "",       ":/icon/rotate3"));

  add(IMG_MIRROR_HORZ,act::make("Mirror horizontally",        "",       ":/icon/mirrorHorz"));
  add(IMG_MIRROR_VERT,act::make("Mirror vertically",          "",       ":/icon/mirrorVert"));

  add(IMG_LINK_CUT,   act::make("Link cut values",            "",       ":/icon/link"));

  add(BG_SELECT,      act::make("Select regions",             "",       ":/icon/selRegion"));
  add(BG_SHOW,        act::make("Show fitted background",     "",       ":/icon/showBackground"));
  add(BG_CLEAR,       act::make("Clear background",           "",       ":/icon/clear"));

  add(REFL_ADD,       act::make("Add reflection",             "Ins",    ":/icon/add"));
  add(REFL_REM,       act::make("Remove reflection",          "Del",    ":/icon/rem"));
  add(REFL_SELECT,    act::make("Select regions",             "",       ":/icon/selRegion"));
  add(REFL_CLEAR,     act::make("Clear background",           "",       ":/icon/clear"));

  add(DLG_POLEFIG,    act::make("Pole figure...",             "Ctrl+P", ":/icon/polefig"));
  add(DLG_DIAGRAM,    act::make("Diagram...",                 "Ctrl+D", ":/icon/diagram"));
  add(DLG_DIFFRGS,    act::make("Diffractograms...",          "Ctrl+F", ":/icon/diffractogram"));

  get(FILES_ADD).onTrigger([&hub]() {
    hub.addFiles();
  });

  get(CORR_ENABLE).onToggle([&hub](bool on) {
    hub.corrEnable(on);
  });

  get(CORR_REM).onTrigger([&hub]() {
    hub.corrRem();
  });

  hub.onSigCorrFileName([this](str name) {
    get(CORR_REM).setEnabled(!name.isEmpty());
  });

  hub.onSigSetsInfo([this](Hub::SetsInfo info) {
    get(CORR_ENABLE).check(info.fp().corrEnabled);
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

str const Acts::IMG_FIX_INTEN   ("imgFixInten");
str const Acts::IMG_STEP_ZOOM   ("imgStepZoom");
str const Acts::IMG_SHOW_OVER   ("imgShowOver");
str const Acts::IMG_SHOW_GAMMA  ("imgShowGamma");

str const Acts::IMG_ROTATE0     ("rotate0");
str const Acts::IMG_ROTATE1     ("rotate1");
str const Acts::IMG_ROTATE2     ("rotate2");
str const Acts::IMG_ROTATE3     ("rotate3");

str const Acts::IMG_MIRROR_HORZ ("mirrorHorz");
str const Acts::IMG_MIRROR_VERT ("mirrorVert");

str const Acts::IMG_LINK_CUT    ("linkCut");

str const Acts::BG_SELECT       ("bgSelect");
str const Acts::BG_SHOW         ("bgShow");
str const Acts::BG_CLEAR        ("bgClear");

str const Acts::REFL_ADD        ("reflAdd");
str const Acts::REFL_REM        ("reflRem");
str const Acts::REFL_SELECT     ("reflSelect");
str const Acts::REFL_CLEAR      ("reflClear");

str const Acts::DLG_POLEFIG     ("outPolefig");
str const Acts::DLG_DIAGRAM     ("outDiagram");
str const Acts::DLG_DIFFRGS     ("outDiffrgs");

//------------------------------------------------------------------------------
}
// eof
