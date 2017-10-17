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

namespace gui {
//------------------------------------------------------------------------------

dcl_sub2_(ViewModelBase, RefHub, l_qt::lst_view)
  ViewModelBase(Hub&);
dcl_end

template <typename Model>
dcl_sub_(ViewModel, ViewModelBase)
  ViewModel(Hub& hub, Model const* model_) : base(hub) {
    setModel(model = model_);
  }

protected:
  Model const* model;
dcl_end

//------------------------------------------------------------------------------

struct Hub;

dcl_sub_(Panel, l_qt::panel)
  Panel();
  Panel(strc title);
dcl_end

//------------------------------------------------------------------------------
}
// eof
