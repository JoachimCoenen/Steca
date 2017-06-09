// (app_lib)

#ifndef APP_LIB_TYP_QT_ACT_H
#define APP_LIB_TYP_QT_ACT_H

#include "../inc/defs_h.h"
#include "../inc/ptr.h"
#include <QAction>
#include <QToolButton>
#include <QHash>

namespace l_qt {
//------------------------------------------------------------------------------

dcl_sub_(act, QAction)
  act();
  act(strc);

  fry_(l::own<act>, make, (strc tx, strc key))
  fry_(l::own<act>, make, (strc tx, strc key, strc iconFile))

  set_(text, (strc))
  mth_(str, text, ())

  set_(tip,  (strc))
  set_(key,  (strc))
  set_(icon, (strc fileName))

  set_(checkable, (bool = true))
  mth_(bool, isCheckable, ())
  set_(check, (bool = true))

  template <typename L>
  set_inl_(onTrigger, (L l), {
    checkable(false);
    connect(this, &QAction::triggered, l);
  })

  template <typename L>
  set_inl_(onToggle, (L l),
    checkable(true);
    connect(this, &QAction::toggled, l);
  )
dcl_end

//------------------------------------------------------------------------------

struct win;

dcl_(acts)
  acts(win&);
  set_(add, (strc hash, l::give_me<act>))
  mth_err_(act&, get, (strc hash))

  cst_(str, QUIT)

protected:
  win& w;
  QHash<str, act> as;
dcl_end

//------------------------------------------------------------------------------

dcl_sub_(actbtn, QToolButton)
  actbtn();
  actbtn(act&);

  set_(action, (act*))

  template <typename L>
  set_inl_(onAct, (L l),
    connect(this, &QAction::triggered, l);
    return *this;
  )
dcl_end

//------------------------------------------------------------------------------
}
#endif
// eof
