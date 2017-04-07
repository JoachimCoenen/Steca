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

#ifndef CORE_FACTORY_H
#define CORE_FACTORY_H

#include <c2/c/ptr.h>
#include <c2/c/str.h>
#include <c2/cpp/exc.hpp>
#include <c2/cpp/hash.hpp>

namespace core {
//-----------------------------------------------------------------------------

template <class ProductBase>
dcl_struct (factory)
  dcl_struct (maker_base)
    virtual ~maker_base() {}
    _abs_mth(c::own<ProductBase>, make, ())
  dcl_struct_end

  template <class Product>
  dcl_struct_sub(maker, maker_base)
    _val (c::own<ProductBase>, make, (), c::owned(new Product))
  dcl_struct_end

  _set_inl (add, (c::strc key, c::give_me<maker_base> m), makers.add(key, m);)

  c::own<ProductBase> make(c::strc key) const may_err {
    try {
      return makers.at(key)->make();
    } catch (std::exception const&) {
      c::err("Factory has no maker ", key);
    }
  }

_protected
  c::hash<c::str, c::own<maker_base>> makers;
dcl_struct_end

//------------------------------------------------------------------------------
}
#endif
// eof
