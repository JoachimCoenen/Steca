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

#ifndef CORE_MODELS_HPP
#define CORE_MODELS_HPP

#include <c2/gui_qt/model.hpp>

namespace core_qt {
//------------------------------------------------------------------------------

def_struct_sub (ModelFiles, c_qt::lst_model)
  explicit ModelFiles();
  _mth (cl_n, cols, ())
  _mth (rw_n, rows, ())
  _mth (c_gui::var*, cell, (rw_n, cl_n))

  _mth_mut (void, check, (rw_n, bool))
  _mth (bool, isChecked, (rw_n))
def_struct_end

//------------------------------------------------------------------------------
}
#endif
// eof
