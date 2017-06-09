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

#include <app_lib/inc/defs_h.h>
#include <app_lib/inc/ptr.h>
#include <QHash>

namespace core {
//-----------------------------------------------------------------------------

template <class ProductBase>
dcl_(factory)
  dcl_(maker_base)
    virtual ~maker_base() {}
    abs_mth_(l::own<ProductBase>, make, ())
  dcl_end

  template <class Product>
  dcl_sub_(maker, maker_base)
    val_(l::own<ProductBase>, make, (), l::owned(new Product))
  dcl_end

  set_inl_(add, (strc key, l::give_me<maker_base> m), makers.insert(key, m.ptr()); )

  l::own<ProductBase> make(strc key) const may_err {
    if (!makers.contains(key))
      err(str("Factory has no maker: %1").arg(key));
    return makers.value(key)->make();
  }

protected:
  QHash<str, maker_base*> makers;
dcl_end

//------------------------------------------------------------------------------
}
#endif
// eof
