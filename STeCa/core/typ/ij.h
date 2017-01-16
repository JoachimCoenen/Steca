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

#ifndef CORE_IJ_H
#define CORE_IJ_H

#include <c/c/def/def_data>

#define DATA_NS    core
#define DATA_NAME  IJ

_c_data
  _c_var(int, i)
  _c_var(int, j)
  _c_con()
  _c_con(int, int)
_c_data_end

_cpp_struct WITH_C_BASE_CONS COMPARABLE EQ_NE
_cpp_struct_end

#undef DATA_NS
#undef DATA_NAME

#endif
// eof
