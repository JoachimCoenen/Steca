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

#ifndef CORE_DATA_HPP
#define CORE_DATA_HPP

#include <c/c/lib/ptr.h>
#include <c/cpp/vec.hpp>


namespace core {
//------------------------------------------------------------------------------

#define _sh_struct _struct typedef c::shared<typ> sh;

#define DATA_NS   data
#define DATA_NAME Set

_sh_struct                      // one read dataset
  _atr(uint, idx)               // this order in File, 1..; 0 = not

  _con(())
_struct_end

#undef  DATA_NAME
#define DATA_NAME File

_sh_struct                      // one file
  _atr(uint, idx)               // this order in Files, 1..; 0 = not
  _atr(c::vec<Set::sh>, sets)

  _con(())
_des
_struct_end

#undef  DATA_NAME
#define DATA_NAME Files

_sh_struct                      // the whole file group
  _atr(c::vec<File::sh>, files)

  _con(())

  _mth_mut(void, addFile, (c::give_me<File>))
  _mth_mut(void, remFile, (uint))
_struct_end

//------------------------------------------------------------------------------
}
#endif
