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

#include <c2/qt/win.hpp>
#include <gui/hub/hub.hpp>

#define NS__ gui
#define ST__ Win

// gui::Win
_struct_sub (c_qt::win) BASE_CONS
  Win();
  Hub hub;
_struct_end

#undef ST__
#undef NS__

#endif
// eof
