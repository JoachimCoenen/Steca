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

#ifndef CORE_SESSION_H
#define CORE_SESSION_H

#include <c2/c/c_def.h>

#define NS__ core
#define ST__ Session

// core_Session
_c_struct
  _atr (int, dummy)
  _c_con ()
_c_struct_end

// core::Session
_cpp_struct
  _con ()
  _mth (int, long_square, (int))
_cpp_struct_end

#undef ST__
#undef NS__

#endif
// eof
