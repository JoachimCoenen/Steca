// (lib/qt)

/** @file
 * an extended Qt widget
 */
#pragma once

#include "../defs.hpp"
#include <QComboBox>

namespace l_qt {
//------------------------------------------------------------------------------

/// a combo
dcl_sub_(cbo, QComboBox)
  cbo();
  /// with a set of options
  cbo(str_vec const&);

  /// add an option
  set_(add, (strc));
  /// add options
  set_(add, (str_vec const&));
dcl_end

//------------------------------------------------------------------------------
}
// eof
