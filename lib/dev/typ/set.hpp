// (dev_lib)

#pragma once
#include "../defs.hpp"
#include <set>

namespace l {
//------------------------------------------------------------------------------

template <typename T>
dcl_reimpl_(set, std::set<T>)
  UB5_(begin, end, erase, clear, find)

  using typename base::iterator;
  using typename base::const_iterator;

  bol_(isEmpty, ())     RET_(base::empty())
  mth_(sz_t, size, ())  RET_(base::size())

  mth_mut_(T const&, add, (T const& t)) {
    base::insert(t);
    return t;
  }

  bol_(contains, (T const& t)) {
    return base::end() != base::find(t);
  }
dcl_end

//------------------------------------------------------------------------------
}
// eof
