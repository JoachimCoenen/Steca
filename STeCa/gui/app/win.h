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

#ifndef GUI_WIN_H
#define GUI_WIN_H

#include <c/qt/win.hpp>
#include <gui/hub/hub.hpp>

#define DATA_NS   gui
#define DATA_NAME Win

_struct_sub (c_qt::win)
  using base::base;
  Win();
  Hub hub;
_struct_sub_end

#undef DATA_NS
#undef DATA_NAME

#endif
// eof
