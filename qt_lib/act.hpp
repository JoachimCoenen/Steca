// (qt_lib)

#ifndef QT_LIB_ACT_HPP
#define QT_LIB_ACT_HPP

#include <dev_lib/inc/defs.hpp>
#include <dev_lib/inc/ptr.hpp>
#include <dev_lib/typ/hash.hpp>

#include <QAction>
#include <QToolButton>

namespace l_qt {
//------------------------------------------------------------------------------

dcl_sub_(act, QAction)
  act(strc);

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

  template <typename L>
  set_(onTrigger, (L l)) SET_(
    setCheckable(false);
    connect(this, &QAction::triggered, l);
  )

  template <typename L>
  set_(onToggle, (L l)) {
    setCheckable(true);
    connect(this, &QAction::toggled, l);
    RTHIS
  }
dcl_end

////------------------------------------------------------------------------------

struct win;

dcl_(acts)
  acts(win&);
  set_(add, (strc hash, l::give_me<act>));
  mth_(act&, get, (strc hash)) may_err;

  cst_(str, QUIT);

protected:
  win& w;
  l::hash<str, act const*> as;
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(actbtn, QToolButton)
  actbtn();
  actbtn(act&);

  set_(action, (act*));

  template <typename L>
  set_(onAct, (L l)) SET_(connect(this, &QAction::triggered, l))
dcl_end

//------------------------------------------------------------------------------
}
#endif
// eof
