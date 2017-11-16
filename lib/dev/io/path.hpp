// (lib/dev)

/** @file
 * file path handling
 */

#pragma once

#include "../defs.hpp"
#include "../inc/str.hpp"
#include "../typ/vec.hpp"

namespace l_io {
//------------------------------------------------------------------------------

dcl_sub_(path, str)
  path(strc);

  mth_(str,  filename, ()); ///< extract filename
  mth_(path, dir,      ()); ///< extract dir
  mth_(path, absolute, ()); ///< convert to absolute pathe

  fry_(str,  cwd, ());      ///< current working dictionary
  voi_(cd,  ());            ///< cd to path
dcl_end

dcl_sub_(path_vec, l::vec<path>)
  using base::base;
  path_vec(str_vec const&);
dcl_end

//------------------------------------------------------------------------------
}
// eof
