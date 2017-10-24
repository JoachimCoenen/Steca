// (qt_lib)

#include "btn.hpp"
#include "act.hpp"
#include "inc/defs.inc"

namespace l_qt {
//------------------------------------------------------------------------------

btn::btn() {
  base::setToolButtonStyle(Qt::ToolButtonIconOnly);
}

btn::btn(act& a) : Self() {
  action(&a);
}

btn::ref btn::action(act *a)
  SET_(base::setDefaultAction(a))

//------------------------------------------------------------------------------

pushbtn::pushbtn() {}
pushbtn::pushbtn(strc tx) {
  text(tx);
}

pushbtn::ref pushbtn::text(strc tx)
  SET_(base::setText(toQt(tx)))

//------------------------------------------------------------------------------
}
// eof
