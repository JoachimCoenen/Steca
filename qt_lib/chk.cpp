// (qt_lib)

#include "chk.hpp"
#include <dev_lib/defs.inc>

#include "act.hpp"
#include "str_inc.hpp"

namespace l_qt {
//------------------------------------------------------------------------------

chk::chk(strc t) {
  text(t);
}

chk::chk(act& act) {
  action(&act);
}

chk::ref chk::text(strc t) SET_(base::setText(toQt(t)))

chk::ref chk::action(act* a) {
  base::setText(a ? toQt(a->text()).toLower() : QString::null);

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

  RTHIS
}

chk::ref chk::check(bool on) SET_(base::setChecked(on))

//------------------------------------------------------------------------------
}
// eof
