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
#include <c2/c/c_cpp>
#include <c2/gui_qt/act.hpp>
#include <c2/gui_qt/btn.hpp>
#include <c2/gui_qt/panel.hpp>

namespace gui {
//------------------------------------------------------------------------------

Win::Win() : hub(), acts(*this) {
  auto &vb = makePanel().makeVBox();

  vb.add(new c_qt::toolbar(c_gui::def::orient::Horz));
  vb.add(new c_qt::actbtn(acts.get(acts.QUIT)));
  vb.add(new c_qt::actbtn(acts.get(acts.SHOW_FILES)));
}

//------------------------------------------------------------------------------
}
// eof
