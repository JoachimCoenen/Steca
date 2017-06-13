// (qt_lib)

#include "chk.h"
#include <dev_lib/inc/defs_cpp.h>
#include "act.h"
#include "str_inc.h"

namespace l_qt {
//------------------------------------------------------------------------------

chk::chk(strc t) {
  text(t);
}

chk::chk(act& act) {
  action(&act);
}

chk::ref chk::text(strc t) {
  base::setText(toQt(t)); RT
}

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

  RT
}

chk::ref chk::check(bool on) {
  base::setChecked(on); RT
}

//------------------------------------------------------------------------------
}
// eof
