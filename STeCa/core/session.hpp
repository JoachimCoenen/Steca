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

#ifndef CORE_SESSION_HPP
#define CORE_SESSION_HPP

#include <c2/h/c_def.h>
#include <c2/c/str.h>
#include <c2/cpp/vec.hpp>

namespace core {
//------------------------------------------------------------------------------

dcl_struct (Session)
  using str_vec = c::vec<c::str>;

  explicit Session();

  enum class eNorm {
    NONE,
    MONITOR, DELTA_MONITOR, DELTA_TIME, BACKGROUND,
  };

  _atr (str_vec, normStrLst)

dcl_struct_end

//------------------------------------------------------------------------------
}
#endif
// eof
