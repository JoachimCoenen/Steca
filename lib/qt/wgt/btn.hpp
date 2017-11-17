// (lib/qt)

/** @file
 * an extended Qt widget
 */
#pragma once

#include "../defs.hpp"
#include <QPushButton>
#include <QToolButton>

namespace l_qt {
//------------------------------------------------------------------------------

struct act;

/// a button with an icon (icon taken from @c act)
dcl_sub_(btn, QToolButton)
  btn();
  btn(act&);
  set_(action, (act*));
dcl_end

/// a pushbutton
dcl_sub_(pushbtn, QPushButton)
  pushbtn();
  pushbtn(strc);
  set_(text, (strc));

  /// a helper to attach a handler
  template <typename L>
  set_(onClick, (L l)) SET_(connect(this, &QPushButton::clicked, l))
dcl_end

//------------------------------------------------------------------------------
}
// eof
