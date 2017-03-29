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

#include "../typ/def.hpp"
#include "../typ/image.hpp"
#include <c2/cpp/map.hpp>

namespace core {
struct Session;

namespace data {
//------------------------------------------------------------------------------

dcl_struct (Meta) SHARED // metadata
  // attribute dictionary
  dcl_struct_reimpl (Dict, c::map<c::str COMMA uint>) SHARED
    using base::size;
    _mth_mut (uint, add, (c::strc))
    _mth_err (uint, at,  (c::strc))
  };

  _atr (Dict::sh,      dict)
  _atr (c::vec<flt32>, vals)

  _atr (flt32, tth) _atr (flt32, omg) _atr (flt32, chi) _atr (flt32, phi)

  Meta(Dict::sh, flt32, flt32, flt32, flt32);
dcl_struct_end

//------------------------------------------------------------------------------
dcl_struct (Set) SHARED   // one dataset
  _atr (uint,      idx)   // this order in File, 1..; 0 = not in File
  _atr (Meta::sh,  meta)
  _atr (Image::sh, image)

  Set(Meta::sh, Image::sh);

  _mth (tth_t,  midTth, ())
  _mth (c::deg, omg, ())
  _mth (c::deg, phi, ())
  _mth (c::deg, chi, ())
  _mth (gma_rge, rgeGma,     (Session const&))
  _mth (gma_rge, rgeGmaFull, (Session const&))
  _mth (tth_rge, rgeTth,     (Session const&))

  _voi (collect, (Session const&, Image const* corr,
                  core::inten_vec&, core::uint_vec&, gma_rge::rc,
                  tth_t minTth, tth_t deltaTth))
dcl_struct_end

//------------------------------------------------------------------------------

struct Files;

dcl_struct (File) SHARED  // one file
  _ref (Files, files)
  _atr (uint, idx)        // this order in Files, 1..; 0 = not in Files
  _atr (c::vec<std::pair<c::str COMMA c::str>>, strs)
  _atr (c::vec<Set::sh>, sets)

  File(Files const&);

  _voi_mut (addSet, (Set::sh))
dcl_struct_end

//------------------------------------------------------------------------------
dcl_struct (Files) SHARED // the whole file group
  _atr (c::vec<File::sh>, files)
  _atr (Meta::Dict::sh,   dict)

  Files();

  _voi_mut (addFile, (data::File::sh))
  _voi_mut (remFile, (uint))
dcl_struct_end

//------------------------------------------------------------------------------
}}
#endif
