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

#ifndef CORE_FIT_METHODS_H
#define CORE_FIT_METHODS_H

#include "fit_fun.hpp"

namespace core { namespace fit {
//------------------------------------------------------------------------------

dcl_iface_(Method)
  Method();
  voi_(fit, (Fun&, Curve::rc))

protected_
  // these pointers are valid during fit() call, for use in callbackY
  ptr_ (Fun,      fun)
  ptr_ (real_vec, xs)

  abs_voi_(approximate, (real*, real const*, real const*, real*, uint,
                          real const*, uint))

  // calculate a vector of y(x)
  voi_(callbackY, (real*, real*, int, int, void*))
dcl_end

//------------------------------------------------------------------------------

dcl_struct_sub_(LinearLeastSquare, Method)
protected_
  voi_(approximate, (real*, real const*, real const*, real*, uint,
                      real const*, uint))
dcl_end

//------------------------------------------------------------------------------

dcl_struct_sub_(LevenbergMarquardt, Method)
protected_
  voi_(approximate, (real*, real const*, real const*, real*, uint,
                      real const*, uint))
  voi_(callbackJacobianLM, (real*, real*, int, int, void*))
dcl_end

//------------------------------------------------------------------------------
}}
#endif
