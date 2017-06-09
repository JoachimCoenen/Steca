// (dev_lib)

#ifndef DEV_LIB_VEC_H
#define DEV_LIB_VEC_H

#include "../inc/defs_h.h"
#include <vector>

namespace l {
//------------------------------------------------------------------------------

template <typename T>
dcl_reimpl_(vec, std::vector<T>)
  using base::base;

  using base::begin;
  using base::end;
  using base::cbegin;
  using base::cend;
  using base::back;
  using base::reserve;
  using base::resize;
  using base::erase;
  using base::clear;
  using base::at;
  using base::data;

  bol_(isEmpty, ())                 { return base::empty();       }
  mth_(sz_t, size, ())              { return to_sz(base::size()); }
  mth_(T const&, first, ())         { return base::front();       }
  mth_(T const&, last,  ())         { return base::back();        }

  set_(add, (T&& t))                { base::push_back(std::move(t));  RT }
  set_(add, (T const& t))           { base::push_back(t);             RT }
  set_(rem, (uint i))               { erase(begin() + i);             RT }
  set_(setAt, (uint i, T&& t))      { base::operator[](i) = std::move(t); RT }
  set_(setAt, (uint i, T const& t)) { base::operator[](i) = t;        RT }

  mth_mut_(T&, refAt, (uint i))     { return base::operator[](i); }
dcl_end

//------------------------------------------------------------------------------
}
#endif
// eof
