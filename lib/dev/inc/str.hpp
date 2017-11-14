// (dev_lib)

#pragma once
#include "../defs.hpp"
#include <string>

//------------------------------------------------------------------------------

dcl_sub_(str, std::string)
  UB3_(base, c_str, length)

  str() {}
  str(base const& that) : base(that) {}

  bol_(isEmpty, ()) RET_(base::empty())

  mth_(flt32, asFlt, ()) may_err;

  static str const null;

  mth_(str, trimmed, ());

  fry_(str, num, (int));
  fry_(str, num, (uint));

private:
  bool empty() const; // disallow
dcl_end

namespace std {

template <> struct hash<str> {
  size_t operator()(strc s) const;
};

}

//------------------------------------------------------------------------------
// eof DOCS
