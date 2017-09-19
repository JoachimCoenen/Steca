// (qt_lib)

#include "font.hpp"
#include "inc/defs.inc"
#include <QWidget>

namespace l_qt {
//------------------------------------------------------------------------------

int mWidth(QWidget const& w, real n) {
  return l::to_int(l::round(n * w.fontMetrics().width('m')));
}

//------------------------------------------------------------------------------
}
// eof
