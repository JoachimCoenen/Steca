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

#pragma once
#include <lib/qt/layout.hpp>
#include <lib/qt/lst.hpp>
#include "../refhub.hpp"

namespace l_qt {
  struct act;
  struct actbtn;
  struct lbl;
  struct ico;
}

namespace gui {
//------------------------------------------------------------------------------

template <typename Model>
dcl_sub_(ViewModel, l_qt::lst_view)
  ViewModel(Model const* model_) {
    setModel(model = model_);
  }

protected:
  Model const* model;
dcl_end

//------------------------------------------------------------------------------

struct Hub;

l_qt::actbtn* btn(l_qt::act&);
l_qt::lbl*    lbl(strc);
l_qt::ico*    ico(strc);

//------------------------------------------------------------------------------

dcl_sub_(Panel, l_qt::panel)
  Panel();
  Panel(strc title);
dcl_end

//------------------------------------------------------------------------------
}
// eof
