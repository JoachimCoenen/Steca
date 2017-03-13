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

#include "acts.hpp"
#include "win.hpp"
#include <c2/c/c_cpp>
#include <QMenu>

namespace gui {
//------------------------------------------------------------------------------

template <typename L>
void f() { L(); }

Acts::Acts(Win& w) : base(w) {
  using act = c_qt::act;

  add(SHOW_FILES, act::make("Show files", "Ctrl+1", ":/icon/link"));
  get(SHOW_FILES)->onAct([](){ TR(88) });
}

c::str const Acts::SHOW_FILES("showFiles");

//------------------------------------------------------------------------------
}
// eof
