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

#ifndef CORE_XY_H
#define CORE_XY_H

#include <c/c/def/def_data>
#include <c/c/def/def_num>

#define DATA_NS    core
#define DATA_NAME  XY

_c_data
  _var (real, x)
  _var (real, y)

  _c_con ()
  _c_con (real, real)
_c_data_end

_cpp_struct
  using base::base;
  COMPARABLE EQ_NE
_cpp_struct_end

#undef DATA_NS
#undef DATA_NAME

#endif
// eof
