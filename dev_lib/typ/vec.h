// (dev_lib)

#ifndef DEV_LIB_VEC_H
#define DEV_LIB_VEC_H

#include "../inc/defs_h.h"
#include "../inc/num.h"
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

  bol_(isEmpty, ())                 VAL_(base::empty())
  mth_(sz_t, size, ())              VAL_(to_sz(base::size()))
  mth_(T const&, first, ())         VAL_(base::front())
  mth_(T const&, last,  ())         VAL_(base::back())

  set_(add, (T&& t))                SET_(base::push_back(std::move(t)))
  set_(add, (T const& t))           SET_(base::push_back(t))
  set_(rem, (uint i))               SET_(erase(begin() + i))
  set_(setAt, (uint i, T&& t))      SET_(base::operator[](i) = std::move(t))
  set_(setAt, (uint i, T const& t)) SET_(base::operator[](i) = t)

  mth_mut_(T&, refAt, (uint i))     VAL_(base::operator[](i))
dcl_end

//------------------------------------------------------------------------------
}
#endif
// eof
