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

#include <c2/c/ptr.h>
#include <c2/c/str.h>
#include <c2/cpp/vec.hpp>
#include <c2/cpp/map.hpp>
#include <c2/cpp/exc.hpp>

namespace core {
#define NS__ data
//------------------------------------------------------------------------------

#define ST__ Meta

// data::Meta
_struct SHARED // metadata
  struct Dict : private c::map<c::str, uint> {
    typedef c::shared<Dict> sh; using base = c::map<c::str, uint>;

    uint size()       const;
    uint add(c::strc);
    uint at(c::strc)  const may_err;
  };

  _atr (Dict::sh,      dict)
  _atr (c::vec<flt32>, vals)
  _atr (flt32, tth) _atr (flt32, omg) _atr (flt32, chi) _atr (flt32, phi)

  _con (Dict::sh, flt32, flt32, flt32, flt32)
_struct_end

#undef  ST__
#define ST__ Set

// data::Set
_struct SHARED                   // one read dataset
  _atr (uint, idx)               // this order in File, 1..; 0 = not
  _atr (Meta, meta)

  _con (Meta::rc)
_struct_end

#undef  ST__
#define ST__ File

// data::File
_struct SHARED                   // one file
  _atr (uint, idx)               // this order in Files, 1..; 0 = not
  _atr (c::vec<Set::sh>, sets)

  _con ()

  _mth_mut(void, addSet, (Set::sh))
  _des ()
_struct_end

#undef  ST__
#define ST__ Files

// data::Files
_struct SHARED                   // the whole file group
  _atr (c::vec<File::sh>, files)
  _atr (Meta::Dict::sh,   dict)

  _con ()

  _mth_mut(void, addFile, (c::give_me<File>))
  _mth_mut(void, remFile, (uint))
_struct_end

#undef  ST__

//------------------------------------------------------------------------------
#undef NS__
}
#endif
