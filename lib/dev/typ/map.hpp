// (dev_lib)

#pragma once
#include "../defs.hpp"
#include "../inc/ptr.hpp"
#include <map>

namespace l {
//------------------------------------------------------------------------------

template <typename K, typename T>
dcl_reimpl_(map, std::map<K COMMA T>)
  using base::base;

  using base::begin;
  using base::end;
  using base::erase;
  using base::clear;
  using base::at;
  using base::find;

  using typename base::iterator;
  using typename base::const_iterator;

  bol_(isEmpty, ())     RET_(base::empty())
  mth_(uint, size, ())  RET_(base::size())

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

template <typename K, typename T>
dcl_sub_(scoped_map, map<K COMMA scoped<T const>>)
  mth_mut_(T const*, add, (K const& k, T const* t)) {
    base::operator[](k).reset(t);
    return t;
  }
dcl_end

//------------------------------------------------------------------------------
}
// eof
