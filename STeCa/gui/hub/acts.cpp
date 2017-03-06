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

#include "acts.h"
#include <gui/app/win.h>
#include <c2/c/c_cpp>
#include <QMenu>
#include <QMenuBar>

namespace gui {
//------------------------------------------------------------------------------

template <typename L>
void f() { L(); }

Acts::Acts(Win& win) {
  using act = c_qt::act;

  auto mb = win.menuBar();
  mb->setNativeMenuBar(false);
  mb->setFixedHeight(0); // "hide"

  auto _ = [mb](pcstr t, pcstr ks) {
    auto a = new act(t); a->key(ks);
    mb->addAction(a);
    return a;
  };

  (quit = _("Quit", "Ctrl+Q"))->onAct([&win]() { win.close(); });

  (showFiles = _("Show files", "Ctrl+1"))->onAct([](){ TR(88) });
}

//------------------------------------------------------------------------------
}
// eof
