// (qt_lib)

#include "font.hpp"
#include "inc/defs.inc"
#include <QWidget>

namespace l_qt {
//------------------------------------------------------------------------------

int mWidth(QWidget const& w, real n) {
  return l::to_int(l::round(n * w.fontMetrics().width('m')));
}

int oWidth(QWidget const& w, uint n) {
  return int(n) * w.fontMetrics().width('0');
}

//------------------------------------------------------------------------------
}
// eof
