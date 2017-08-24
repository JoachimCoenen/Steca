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

#include "app.hpp"
#include "../manifest.h"
#include "win.hpp"

namespace gui {
//------------------------------------------------------------------------------

App::App(int& argc, char* argv[]) : base(argc, argv) {
  setApplicationName(APPLICATION_NAME);
  setApplicationVersion(
    #include "VERSION"
  );
  setOrganizationName(ORGANIZATION_NAME);
  setOrganizationDomain(ORGANIZATION_DOMAIN);
}

int App::exec() {
  Win w;
  return base::safeExec(&w);
}

//------------------------------------------------------------------------------
}
// eof