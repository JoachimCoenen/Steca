// (qt_lib)

#include "ico.hpp"
#include "inc/defs.inc"
#include <QIcon>

namespace l_qt {
//------------------------------------------------------------------------------

ico::ico() : Self(str::null) {}

ico::ico(strc t) {
  pix(t);
}

ico::ref ico::pix(strc fileName) {
  auto&& h = sizeHint().height();
  setPixmap(QIcon(toQt(fileName)).pixmap(QSize(h, h)));

  RTHIS;
}

//------------------------------------------------------------------------------
}
// eof
