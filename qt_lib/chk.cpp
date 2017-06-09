// (app_lib)

#include "chk.h"
#include "../inc/defs_cpp.h"
#include "act.h"

namespace l_qt {
//------------------------------------------------------------------------------

chk::chk() : chk(str::null) {}

chk::chk(strc t) {
  text(t);
}

chk::chk(act& act) {
  action(&act);
}

chk::ref chk::text(strc text) {
  base::setText(text);
  return *this;
}

chk::ref chk::action(act* a) {
  text(a ? a->text().toLower() : str::null);

  disconnect(con1);
  disconnect(con2);

  if (a) {
    base::setToolTip(a->toolTip());
    base::setChecked(a->isChecked());

    con1 = connect(this, &Self::toggled, [a](bool on) {
      a->check(on);
    });

    con2 = connect(a, &act::toggled, [this](bool on) {
      check(on);
    });
  }

  return *this;
}

chk::ref chk::check(bool on) {
  base::setChecked(on);
  return *this;
}

//------------------------------------------------------------------------------
}
// eof
