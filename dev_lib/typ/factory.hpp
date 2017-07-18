// (dev_lib)

#pragma once

#include "../defs.hpp"
#include "../inc/exc.hpp"
#include "../inc/ptr.hpp"
#include "../typ/hash.hpp"

namespace l {
//-----------------------------------------------------------------------------

template <typename ProductBase>
dcl_(factory)
  dcl_(maker_base)
    virtual ~maker_base() {}
    virtual mth_(l::own<ProductBase>, make, ()) = 0;
  dcl_end

  template <typename Product>
  set_(addMaker, (strc key)) SET_(makers.add(key, new maker<Product>()))

  mth_(l::own<ProductBase>, make, (strc key)) may_err {
    if (!makers.contains(key))
      l::err(CAT("Factory has no maker: ", key));
    return makers.at(key)->make();
  }

protected:
  template <class Product>
  dcl_sub_(maker, maker_base)
    mth_(l::own<ProductBase>, make, ()) VAL_(l::owned(new Product))
  dcl_end

  l::hash<str, maker_base const*> makers;
dcl_end

//------------------------------------------------------------------------------
}
// eof
