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
#include <lib/qt/act.hpp>

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

  // diffractogram
  cst_(str, DIFF_ZOOM);
  cst_(str, DIFF_ALL_DSETS);
  cst_(str, DIFF_FIX_INTEN);

  // image
  cst_(str, IMG_FIX_INTEN);
  cst_(str, IMG_STEP_ZOOM);
  cst_(str, IMG_SHOW_OVER);
  cst_(str, IMG_SHOW_GAMMA);

  cst_(str, IMG_ROTATE0);
  cst_(str, IMG_ROTATE1);
  cst_(str, IMG_ROTATE2);
  cst_(str, IMG_ROTATE3);

  cst_(str, IMG_MIRROR_HORZ);
  cst_(str, IMG_MIRROR_VERT);

  cst_(str, IMG_LINK_CUT);

  // background
  cst_(str, BG_SELECT);
  cst_(str, BG_SHOW);
  cst_(str, BG_CLEAR);

  // reflection
  cst_(str, REFL_ADD);
  cst_(str, REFL_REM);
  cst_(str, REFL_SELECT);
  cst_(str, REFL_CLEAR);

  // outputs
  cst_(str, DLG_POLEFIG);
  cst_(str, DLG_DIAGRAM);
  cst_(str, DLG_DIFFRGS);
dcl_end

//------------------------------------------------------------------------------
}
// eof
