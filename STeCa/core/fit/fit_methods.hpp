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

dcl_iface (Method)
  Method();
  _voi (fit, (Fun&, Curve::rc))

_protected
  // these pointers are valid during fit() call, for use in callbackY
  _ptr (Fun,      fun)
  _ptr (real_vec, xs)

  _abs_voi (approximate, (real*, real const*, real const*, real*, uint,
                          real const*, uint))

  // calculate a vector of y(x)
  _voi (callbackY, (real*, real*, int, int, void*))
};

//------------------------------------------------------------------------------

dcl_struct_sub(LinearLeastSquare, Method)
_protected
  _voi (approximate, (real*, real const*, real const*, real*, uint,
                      real const*, uint))
dcl_struct_end

//------------------------------------------------------------------------------

dcl_struct_sub(LevenbergMarquardt, Method)
_protected
  _voi (approximate, (real*, real const*, real const*, real*, uint,
                      real const*, uint))
  _voi (callbackJacobianLM, (real*, real*, int, int, void*))
dcl_struct_end

//------------------------------------------------------------------------------
}}
#endif
