// (dev_lib)

#pragma once
#include "../defs.hpp"
#include "../inc/str.hpp"
#include "../typ/vec.hpp"

namespace l_io {
//------------------------------------------------------------------------------

dcl_sub_(path, str)
  path(strc);

  mth_(str,  filename, ());
  mth_(path, dir,      ());
  mth_(path, absolute, ());

  fry_(str,  cwd, ());
  voi_(cd,  ());
dcl_end

dcl_sub_(path_vec, l::vec<path>)
  using base::base;
  path_vec(str_vec const&);
dcl_end

//------------------------------------------------------------------------------
}
// eof
