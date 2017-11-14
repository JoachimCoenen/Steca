/*******************************************************************************
 * Steca2 - StressTextureCalculator ver. 2
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
#include <lib/qt/view/lst.hpp>
#include <core/calc/reflection.hpp>
#include "../refhub.hpp"

namespace gui {
//------------------------------------------------------------------------------

dcl_sub_(ModelReflections, l_qt::lst_model)
  ModelReflections();

  enum { clID, clTYPE };

  mth_(cl_n, cols, ());
  mth_(rw_n, rows, ());

  mth_(str,       head, (cl_n));
  mth_(l_qt::var, cell, (rw_n, cl_n));

  mth_(str,     name,  (rw_n));
  mth_(str_vec, names, ());

private:
  core::calc::reflection_vec const* rs;
dcl_end

//------------------------------------------------------------------------------

template <typename Model>
dcl_sub2_(HubView, RefHub, l_qt::lst_view)
  HubView(Hub& hub, Model const* model_) : RefHub(hub) {
    setModel(model = model_);
  }

protected:
  Model const* model;
dcl_end

//------------------------------------------------------------------------------
}
// eof DOCS
