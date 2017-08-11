// (dev_lib)

#pragma once
#include "../defs.hpp"
#include "../inc/str.hpp"
#include "../typ/vec.hpp"

//------------------------------------------------------------------------------
// useful vecs, forward-decld in defs.hpp

dcl_sub_(str_vec, l::vec<str>)
  using base::base;
  str_vec() {} // for Mac

  fry_(str_vec, split, (strc));
dcl_end

dcl_sub_(int_vec, l::vec<int>)
  using base::base;
  int_vec() {} // for Mac
dcl_end

dcl_sub_(uint_vec, l::vec<uint>)
  using base::base;
  uint_vec() {} // for Mac
dcl_end

dcl_sub_(real_vec, l::vec<real>)
  using base::base;
  real_vec() {} // for Mac
dcl_end

//------------------------------------------------------------------------------
// eof
