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

#pragma once
#include <qt_lib/act.hpp>

namespace gui {
//------------------------------------------------------------------------------

struct Hub;
struct Win;

dcl_sub_(Acts, l_qt::acts)
  Acts(Hub&, Win&);

  cst_(str, SHOW_FILES);
  cst_(str, SHOW_DATASETS);
  cst_(str, SHOW_METADATA);

  cst_(str, SESSION_LOAD);
  cst_(str, SESSION_SAVE);

  cst_(str, FILES_ADD);
  cst_(str, FILES_REM);
  cst_(str, CORR_ENABLE);
  cst_(str, CORR_REM);

  cst_(str, DIFF_ZOOM);
  cst_(str, DIFF_ALL_DSETS);
  cst_(str, DIFF_FIX_INTEN);
dcl_end

//------------------------------------------------------------------------------
}
// eof
