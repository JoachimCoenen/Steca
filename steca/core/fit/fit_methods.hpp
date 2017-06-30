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

#include "fit_fun.hpp"

namespace core { namespace fit {
//------------------------------------------------------------------------------

dcl_base_(Method)
  Method();
  mth_(void, fit, (Fun&, Curve::rc));

protected:
  // these pointers are valid during fit() call, for use in callbackY
  ptr_ (Fun,      fun);
  ptr_ (real_vec, xs);

  virtual mth_(void, approximate, (real*, real const*, real const*, real*, uint,
                               real const*, uint)) = 0;

  // calculate a vector of y(x)
  mth_(void, callbackY, (real*, real*, int, int, void*));
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(LinearLeastSquare, Method)
protected:
  mth_(void, approximate, (real*, real const*, real const*, real*, uint,
                      real const*, uint));
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(LevenbergMarquardt, Method)
protected:
  mth_(void, approximate, (real*, real const*, real const*, real*, uint,
                      real const*, uint));
  mth_(void, callbackJacobianLM, (real*, real*, int, int, void*));
dcl_end

//------------------------------------------------------------------------------
}}
