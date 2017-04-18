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

#ifndef GUI_ACTS_HPP
#define GUI_ACTS_HPP

#include <c2/gui_qt/act.hpp>

namespace gui {
//------------------------------------------------------------------------------

struct Win;

dcl_struct_sub_(Acts, c_qt::acts)
  Acts(Win&);

  cst_(c::str, SHOW_FILES)
  cst_(c::str, SHOW_DATASETS)
  cst_(c::str, SHOW_METADATA)

  cst_(c::str, FILES_ADD)
  cst_(c::str, FILES_REM)
  cst_(c::str, CORR_ENABLE)
  cst_(c::str, CORR_REM)

  cst_(c::str, DIFF_ZOOM)
  cst_(c::str, DIFF_ALL_DSETS)
  cst_(c::str, DIFF_FIX_INTEN)
dcl_end

//------------------------------------------------------------------------------
}
#endif
// eof
