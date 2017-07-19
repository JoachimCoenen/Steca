// (dev_lib)

#pragma once

#include "defs.hpp"
#include <string>

//------------------------------------------------------------------------------

dcl_sub_(str, std::string)
  using base::base;
  str() {}
  str(base const& that) : base(that) {}

  using base::c_str;
  using base::length;

  mth_(bool, isEmpty, ()) VAL_(base::empty())

  mth_(flt32, asFlt, ()) may_err;

  static str const null;

  mth_(str, trimmed, ());

private:
  bool empty() const; // disallow
dcl_end

using pcstr = char const*;
using strc  = str const&;

namespace std {

template <> struct hash<str> {
  size_t operator()(strc s) const;
};

}

//------------------------------------------------------------------------------
// eof
