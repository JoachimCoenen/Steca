// (dev_lib)

#pragma once

#include "../defs.hpp"

namespace l_io {
//------------------------------------------------------------------------------

dcl_sub_(path, str)
  path(strc);

  mth_(str,  filename, ());
  mth_(path, dir,      ());
  mth_(path, absolute, ());

  fry_(str,  cwd, ());
  act_(cd,  ());
dcl_end

//------------------------------------------------------------------------------
}
// eof
