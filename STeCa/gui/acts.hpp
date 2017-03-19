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

dcl_struct_sub (Acts, c_qt::acts)
  Acts(Win&);

  _cst (c::str, SHOW_FILES)
  _cst (c::str, SHOW_DATASETS)
  _cst (c::str, SHOW_METADATA)

  _cst (c::str, FILES_ADD)
  _cst (c::str, FILES_REM)
  _cst (c::str, CORR_ENABLE)
  _cst (c::str, CORR_REM)

  _cst (c::str, DIFF_ZOOM)
  _cst (c::str, DIFF_ALL_DSETS)
  _cst (c::str, DIFF_FIX_INTEN)
dcl_struct_end

//------------------------------------------------------------------------------
}
#endif
// eof
