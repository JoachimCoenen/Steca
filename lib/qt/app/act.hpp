// (lib/qt)

/** @file
 * actions: extended from QAction
 */

#pragma once

#include "../defs.hpp"
#include <lib/dev/inc/ptr.hpp>
#include <lib/dev/inc/str.hpp>
#include <lib/dev/typ/hash.hpp>
#include <QAction>

namespace l_qt {
//------------------------------------------------------------------------------

/** action: has a (display) text, tooltip, hotkey combination, icon resource;
 * can be checkable - then be checked
 */
dcl_sub_(act, QAction)
  act(strc);

  fry_(l::own<act>, make, (strc tx));
  fry_(l::own<act>, make, (strc tx, strc key));
  fry_(l::own<act>, make, (strc tx, strc key, strc iconFile));

  set_(text, (strc));
  mth_(str, text, ());

  set_(tip,  (strc));
  set_(key,  (strc));
  set_(icon, (strc fileName));

  bol_(isCheckable, ());
  set_(setCheckable, (bool = true));
  set_(check, (bool = true));

  /// sets a handler for the action; makes the action non-checkable
  template <typename L>
  set_(onTrigger, (L l)) SET_(
    setCheckable(false);
    connect(this, &QAction::triggered, l);
  )

  /// sets a handler for the action; makes the action checkable
  template <typename L>
  set_(onToggle, (L l)) SET_(
    setCheckable(true);
    connect(this, &QAction::toggled, l);
  )
dcl_end

//------------------------------------------------------------------------------

struct win;

/** a set of actions attached to a window; actions keyed by a string (hash)
 */
dcl_(acts)
  /// actions are attached to a window
  acts(win&);
 ~acts();

  /// add an action to the set
  set_(add, (strc hash, l::give_me<act>));
  /// access an action
  mth_(act&, get, (strc hash)) may_err;

#ifndef Q_OS_OSX
  /// pre-defined key for a pre-defined action
  cst_(str, FULL_SCREEN);
#endif

  /// pre-defined key for a pre-defined action
  cst_(str, ABOUT);
  /// pre-defined key for a pre-defined action
  cst_(str, QUIT);

protected:
  win& w;
  l::hash<str, act const*> as;
dcl_end

//------------------------------------------------------------------------------
}
// eof
