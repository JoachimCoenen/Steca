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

#include "hub.hpp"
#include "win.hpp"
#include <dev_lib/defs.inc>
#include <dev_lib/io/path.hpp>
#include <qt_lib/dlg_file.hpp>

namespace gui {
//------------------------------------------------------------------------------

Hub::Hub(Win& win_) : win(win_), acts(*this, win_) {}

Hub::ref Hub::addFiles() {
  auto names = l_qt::dlgOpenFiles(&mut(win), "Add files", l_io::path::cwd(),
                "Data files (*.dat *.mar*);;All files (*.*)");

  if (!names.isEmpty()) {
    l_io::path(names.at(0)).absolute().cd();
// session:    hub_.addFiles(fileNames);
  }
  RTHIS
}

//------------------------------------------------------------------------------
}
// eof
