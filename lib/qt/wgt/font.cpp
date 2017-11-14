// (qt_lib)

#include "font.hpp"
#include "../defs.inc"
#include <QWidget>

namespace l_qt {
//------------------------------------------------------------------------------

int width_m(QWidget const& w, real n) {
  return l::to_int(l::round(n * w.fontMetrics().width('m')));
}

int width_0(QWidget const& w, real n) {
  return l::to_int(l::round(n * w.fontMetrics().width('0')));
}

//------------------------------------------------------------------------------
}
// eof DOCS
