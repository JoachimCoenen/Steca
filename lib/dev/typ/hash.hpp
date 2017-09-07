// (dev_lib)

#pragma once
#include "../defs.hpp"
#include "../inc/ptr.hpp"
#include <unordered_map>

namespace l {
//------------------------------------------------------------------------------

template <typename K, typename T>
dcl_reimpl_(hash, std::unordered_map<K COMMA T>)
  using base::begin;
  using base::end;
  using base::empty;
  using base::erase;
  using base::clear;
  using base::at;
  using base::find;

  bol_(isEmpty, ())     RET_(base::empty())
  mth_(uint, size, ())  RET_(base::size())

  mth_mut_(T const&, add, (K const& k, T const& t)) {
    base::insert(std::make_pair(k, t));
    return t;
  }

  bol_(contains, (K const& k)) {
    return base::end() != base::find(k);
  }

  mth_mut_(T, take, (K const& k)) may_err {
    T t = at(k); erase(k);
    return t;
  }
dcl_end

//------------------------------------------------------------------------------
}
// eof
