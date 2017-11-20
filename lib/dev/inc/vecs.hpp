// (lib/dev)

/** @file
 * useful vectors, forward-declared in defs.hpp;
 * Mac requires the explicit default constructors
 */

#pragma once

#include "../defs.hpp"
#include "../inc/str.hpp"
#include "../typ/vec.hpp"

//------------------------------------------------------------------------------

/// vector of @c str
dcl_sub_(str_vec, l::vec<str>)
  using base::base;
  str_vec() {}
  str_vec(pcstr[], uint n);

  /// splits a string by white space
  fry_(str_vec, split, (strc));
dcl_end

/// vector of @c bool
dcl_sub_(bol_vec, l::vec<bool>)
  using base::base;
  bol_vec() {}
dcl_end

/// vector of @c int
dcl_sub_(int_vec, l::vec<int>)
  using base::base;
  int_vec() {}
dcl_end

/// vector of @c uint
dcl_sub_(uint_vec, l::vec<uint>)
  using base::base;
  uint_vec() {}
dcl_end

/// vector of @c real
dcl_sub_(real_vec, l::vec<real>)
  using base::base;
  real_vec() {}
dcl_end

//------------------------------------------------------------------------------
// eof
