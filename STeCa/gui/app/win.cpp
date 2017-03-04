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

#include "win.h"
#include <c2/gui_qt/btn.hpp>
#include <c2/c/c_cpp>

namespace gui {
//------------------------------------------------------------------------------

Win::Win() : hub() {
  using btn = c_qt::btn;
  //  makeSplit(c_gui::def::orient::Horz);
  auto &p = makePanel();
  auto &b = p.makeBox(c_gui::def::orient::Vert).hbox();
  b.add(new btn("Hello"));
  auto &v = b.vbox();
  b.add(new btn("World"));
  for_i (3)
    v.add(new btn(c::unsafe::str_frm("%d", i+1)));
}

//------------------------------------------------------------------------------
}
// eof
