// (lib/dev)

/** @file
 * map; map of scoped pointers
 */

#pragma once

#include "../defs.hpp"
#include "../inc/ptr.hpp"
#include <map>

namespace l {
//------------------------------------------------------------------------------

template <typename K, typename T>
dcl_reimpl_(map, std::map<K COMMA T>)
  UB6_(begin, end, erase, clear, at, find)

  using typename base::iterator;
  using typename base::const_iterator;

  bol_(isEmpty, ())     RET_(base::empty())
  mth_(sz_t, size, ())  RET_(sz_t(base::size()))

  mth_mut_(T const&, add, (K const& k, T const& t)) {
    base::insert(std::make_pair(k,t));
    return t;
  }

  bol_(contains, (K const& k)) {
    return base::end() != base::find(k);
  }

  mth_mut_(T, take, (K const& k)) may_err {
    T t = at(k);
    erase(k);
    return t;
  }
dcl_end

//------------------------------------------------------------------------------
}
// eof
