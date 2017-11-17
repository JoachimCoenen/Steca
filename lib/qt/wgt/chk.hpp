// (lib/qt)

/** @file
 * an extended Qt widget
 */
#pragma once

#include "../defs.hpp"
#include <lib/dev/inc/str.hpp>
#include <QCheckBox>

namespace l_qt {
//------------------------------------------------------------------------------

struct act;

dcl_sub_(chk_base, QCheckBox)
  chk_base(strc);
  set_(text, (strc));
dcl_end

/// a checkbox
dcl_sub_(chk, chk_base)
  using base::base;
  chk(act&, strc = str::null);
  chk(strc = str::null);

  /// connect an action
  set_(action, (act*));
  /// change state
  set_(check,  (bool = true));

protected:
  QMetaObject::Connection con1, con2;
dcl_end

/// a tri-state checkbox
dcl_sub_(triChk, chk_base)
  triChk(strc);

  enum class eState {off, part, on};
  /// change state
  set_(set, (eState));
  /// get state
  mth_(eState, get, ());
dcl_end

//------------------------------------------------------------------------------
}
// eof
