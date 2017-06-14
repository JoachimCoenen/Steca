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

#include <dev_lib/inc/defs_h.h>
#include <dev_lib/inc/exc.h>
#include <dev_lib/inc/ptr.h>
#include <dev_lib/typ/hash.h>

namespace core {
//-----------------------------------------------------------------------------

template <class ProductBase>
dcl_(factory)
  dcl_(maker_base)
    virtual ~maker_base() {}
    virtual mth_(l::own<ProductBase>, make, ()) = 0;
  dcl_end

  template <class Product>
  dcl_sub_(maker, maker_base)
    mth_(l::own<ProductBase>, make, ()) VAL_(l::owned(new Product))
  dcl_end

  set_(add, (strc key, l::give_me<maker_base> m)) SET_(makers.add(key, m.ptr()))

  mth_(l::own<ProductBase>, make, (strc key)) may_err {
    if (!makers.contains(key))
      l::err(CAT("Factory has no maker: ", key));
    return makers.at(key)->make();
  }

protected:
  l::hash<str, maker_base const*> makers;
dcl_end

//------------------------------------------------------------------------------
}
#endif
// eof
