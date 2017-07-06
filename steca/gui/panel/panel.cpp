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

#include "panel.hpp"
#include <dev_lib/defs.inc>

#include "../hub.hpp"
#include <qt_lib/lbl.hpp>

namespace gui {
//------------------------------------------------------------------------------

Panel::Panel(Hub& hub) : Panel(str::null, hub) {}

Panel::Panel(strc title, Hub& hub_) : hub(hub_), vb(base::vb()) {
  if (!title.isEmpty())
    vb.add(new l_qt::lbl(title));
}

//------------------------------------------------------------------------------
}
// eof
