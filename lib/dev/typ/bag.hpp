// (dev_lib)

#pragma once
#include "../defs.hpp"
#include <unordered_set>

namespace l {
//------------------------------------------------------------------------------

template <typename T>
dcl_reimpl_(bag, std::unordered_set<T>)
  using base::base;

  using base::begin;
  using base::end;
  using base::erase;
  using base::clear;
  using base::find;

  using typename base::iterator;
  using typename base::const_iterator;

  bol_(isEmpty, ())     RET_(base::empty())
  mth_(uint, size, ())  RET_(base::size())

  bol_(contains, (T const& t)) {
    return base::end() != base::find(t);
  }

  mth_mut_(T const&, add, (T const& t)) {
    base::insert(t);
    return t;
  }

  bol_mut_(remove, (T const& t)) {
    auto&& it = base::find(t);
    if (base::end() == it)
      return false;
    base::erase(it);
    return true;
  }

  bol_mut_(set, (T const& t, bool on)) {
    if (!on)
      return remove(t);
    if (contains(t))
      return false;
    add(t);
    return true;
  }
dcl_end

//------------------------------------------------------------------------------
}
// eof
